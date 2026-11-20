"""
transaction.py

Defines Transaction, one line of the Merged Daily Transaction file.
"""

from file_formats import (USERNAME_WIDTH, PRICE_WIDTH, parse_money, split_off_right_field)

CREATE, DELETE, SELL, BUY, REFUND, ADD_CREDIT, END_OF_SESSION = (
    "01", "02", "03", "04", "05", "06", "00")


class Transaction:
    """One parsed transaction. Fields that do not apply to a code are left as None."""

    def __init__(self, code, raw_line):
        """Create an empty transaction of the given code; parse() fills in the fields."""
        self.code = code
        self.raw_line = raw_line
        self.username = None      # buyer, or the only user in the transaction
        self.user_type = None
        self.seller = None
        self.game_name = None
        self.amount_cents = 0     # credit, price or refund amount

    @classmethod
    def parse(cls, line):
        """Parse a fixed-width transaction line according to its two-digit code."""
        code = line[0:2]
        txn = cls(code, line)
        if code in (CREATE, DELETE, ADD_CREDIT, END_OF_SESSION):
            # XX UUUUUUUUUUUUUUU TT CCCCCCCCC
            txn.username = line[3:3 + USERNAME_WIDTH].strip()
            txn.user_type = line[19:21]
            txn.amount_cents = parse_money(line[22:31])
        elif code == SELL:
            # XX GAMENAME SSSSSSSSSSSSSSS PPPPPP  (fields after the name are read from the right)
            rest, price = split_off_right_field(line[3:], PRICE_WIDTH)
            name, seller = split_off_right_field(rest, USERNAME_WIDTH)
            txn.game_name, txn.seller, txn.amount_cents = name.strip(), seller, parse_money(price)
        elif code == BUY:
            # XX GAMENAME SSSSSSSSSSSSSSS UUUUUUUUUUUUUUU PPPPPP
            rest, price = split_off_right_field(line[3:], PRICE_WIDTH)
            rest, buyer = split_off_right_field(rest, USERNAME_WIDTH)
            name, seller = split_off_right_field(rest, USERNAME_WIDTH)
            txn.game_name, txn.seller, txn.username = name.strip(), seller, buyer
            txn.amount_cents = parse_money(price)
        elif code == REFUND:
            # XX UUUUUUUUUUUUUUU SSSSSSSSSSSSSSS CCCCCCCCC
            txn.username = line[3:3 + USERNAME_WIDTH].strip()
            txn.seller = line[19:19 + USERNAME_WIDTH].strip()
            txn.amount_cents = parse_money(line[35:44])
        else:
            raise ValueError(f"unknown transaction code '{code}'")
        return txn
