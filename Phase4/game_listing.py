"""
game_listing.py

Defines GameListing, one line of the Available Games file.
"""

from file_formats import (GAME_NAME_WIDTH, USERNAME_WIDTH, PRICE_WIDTH,
                          pad_right, parse_money, format_money, split_off_right_field)


class GameListing:
    """A game offered for sale: its name, seller's username and price in cents."""

    def __init__(self, game_name, seller, price_cents):
        """Create a listing from already-parsed values."""
        self.game_name = game_name
        self.seller = seller
        self.price_cents = price_cents

    @classmethod
    def from_file_line(cls, line):
        """Build a listing from a line: game name, seller (15), price (6), read from the right."""
        rest, price = split_off_right_field(line, PRICE_WIDTH)
        name, seller = split_off_right_field(rest, USERNAME_WIDTH)
        return cls(name.strip(), seller, parse_money(price))

    def to_file_line(self):
        """Return this listing as a fixed-width file line (no newline)."""
        return (pad_right(self.game_name, GAME_NAME_WIDTH) + " "
                + pad_right(self.seller, USERNAME_WIDTH) + " "
                + format_money(self.price_cents, PRICE_WIDTH))
