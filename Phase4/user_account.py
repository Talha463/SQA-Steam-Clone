"""
user_account.py

Defines UserAccount, one line of the Current User Accounts file.
"""

from file_formats import (USERNAME_WIDTH, USER_TYPE_WIDTH, CREDIT_WIDTH,
                          pad_right, parse_money, format_money)


class UserAccount:
    """A user's name, account type (AA/FS/BS/SS) and credit balance in cents."""

    def __init__(self, username, user_type, credit_cents):
        """Create an account from already-parsed values."""
        self.username = username
        self.user_type = user_type
        self.credit_cents = credit_cents

    @classmethod
    def from_file_line(cls, line):
        """Build an account from a fixed-width line: UUUUUUUUUUUUUUU TT CCCCCCCCC."""
        username = line[0:USERNAME_WIDTH].strip()
        user_type = line[16:16 + USER_TYPE_WIDTH]
        credit = parse_money(line[19:19 + CREDIT_WIDTH])
        return cls(username, user_type, credit)

    def to_file_line(self):
        """Return this account as a fixed-width file line (no newline)."""
        return (pad_right(self.username, USERNAME_WIDTH) + " "
                + self.user_type + " "
                + format_money(self.credit_cents, CREDIT_WIDTH))

    def can_afford(self, amount_cents):
        """Return True if paying amount_cents would not make the balance negative."""
        return self.credit_cents >= amount_cents
