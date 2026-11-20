"""
master_files.py

Reads and writes the three master files (accounts, available games, games collection).
"""

from error_log import FatalError
from file_formats import (USERNAME_WIDTH, GAME_NAME_WIDTH, END_MARKER, pad_right,
                          split_off_right_field)
from game_listing import GameListing
from user_account import UserAccount


class MasterFiles:
    """In-memory copy of the three master files, with load and save operations."""

    def __init__(self):
        """Start with no users, no listings and no owned games."""
        self.accounts = {}        # username -> UserAccount
        self.listings = []        # list of GameListing
        self.collection = []      # list of (owner_username, game_name)

    def load(self, accounts_file, games_file, collection_file):
        """Read all three old master files; raise FatalError if one cannot be read."""
        for line in self._read_lines(accounts_file):
            account = UserAccount.from_file_line(line)
            self.accounts[account.username] = account
        for line in self._read_lines(games_file):
            self.listings.append(GameListing.from_file_line(line))
        for line in self._read_lines(collection_file):
            # Collection lines are GAMENAME then OWNER (15), as the specification defines.
            game, owner = split_off_right_field(line, USERNAME_WIDTH)
            self.collection.append((owner, game.strip()))

    def save(self, accounts_file, games_file, collection_file):
        """Write the three new master files, each ending with an END line."""
        self._write_lines(accounts_file,
                          [a.to_file_line() for a in self.accounts.values()],
                          pad_right(END_MARKER, USERNAME_WIDTH))
        self._write_lines(games_file,
                          [g.to_file_line() for g in self.listings],
                          pad_right(END_MARKER, GAME_NAME_WIDTH))
        self._write_lines(collection_file,
                          [pad_right(game, GAME_NAME_WIDTH) + " " + pad_right(owner, USERNAME_WIDTH)
                           for owner, game in self.collection],
                          pad_right(END_MARKER, GAME_NAME_WIDTH))

    def find_listing(self, game_name, seller):
        """Return the listing with this name and seller, or None."""
        for listing in self.listings:
            if listing.game_name == game_name and listing.seller == seller:
                return listing
        return None

    def _read_lines(self, filename):
        """Return the data lines of a master file, stopping at its END line."""
        try:
            with open(filename, "r", newline="") as handle:
                lines = [raw.rstrip("\r\n") for raw in handle]
        except OSError as problem:
            raise FatalError(f"cannot read file ({problem.strerror}) in {filename}")
        data = []
        for line in lines:
            if line[0:len(END_MARKER)] == END_MARKER:
                break
            data.append(line)
        return data

    def _write_lines(self, filename, lines, end_line):
        """Write lines followed by the END line, using \\n line endings on every platform."""
        with open(filename, "w", newline="") as handle:
            for line in lines + [end_line]:
                handle.write(line + "\n")
