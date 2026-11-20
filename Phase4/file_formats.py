"""
file_formats.py

Fixed-width field helpers shared by every file the Back End reads or writes.
The layout follows the project specification (not any one Front End), so any Front End
that follows the spec works. If the specification's widths change, edit only this file.

Money is stored internally as an integer number of cents to avoid float errors.
"""

USERNAME_WIDTH = 15
USER_TYPE_WIDTH = 2
CREDIT_WIDTH = 9
GAME_NAME_WIDTH = 26      # width used when WRITING files (gives the spec's 49 / 42 char lines)
PRICE_WIDTH = 6
END_MARKER = "END"


def pad_right(text, width):
    """Return text left-justified and space-filled (or cut) to exactly width characters."""
    return text[:width].ljust(width)


def split_off_right_field(text, width):
    """
    Remove the last field (width characters) and its separating space from the end of text.
    Returns (remaining_text, field). Reading variable-position fields from the right means
    the game-name width used by another Front End does not matter.
    """
    field = text[-width:].strip()
    remaining = text[:-(width + 1)]
    return remaining, field


def parse_money(text):
    """Convert a money field such as '019.99' into integer cents (1999)."""
    dollars, _, fraction = text.strip().partition(".")
    fraction = (fraction + "00")[:2]
    return int(dollars or "0") * 100 + int(fraction)


def format_money(cents, width):
    """Convert integer cents into a zero-filled money field such as '019.99'."""
    return f"{cents // 100}.{cents % 100:02d}".rjust(width, "0")
