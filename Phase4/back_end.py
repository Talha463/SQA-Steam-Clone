"""
back_end.py

Applies the Merged Daily Transaction file to the master files.
"""

import transaction as codes
from game_listing import GameListing
from master_files import MasterFiles
from transaction import Transaction
from user_account import UserAccount


class BackEnd:
    """Runs one overnight batch: old master files + merged transactions -> new master files."""

    def __init__(self, master_files, error_log):
        """Keep the master data to update and the log used to report failed constraints."""
        self.master = master_files
        self.log = error_log

    def process_file(self, merged_transaction_file):
        """Read the merged transaction file one line at a time and apply each transaction."""
        with open(merged_transaction_file, "r", newline="") as handle:
            for raw in handle:
                line = raw.rstrip("\r\n")
                if line.strip():
                    self.apply(Transaction.parse(line))

    def apply(self, txn):
        """Send a transaction to the handler for its code."""
        handlers = {
            codes.CREATE: self.apply_create,
            codes.ADD_CREDIT: self.apply_add_credit,
            codes.SELL: self.apply_sell,
            codes.BUY: self.apply_buy,
            codes.DELETE: self.apply_delete,
            codes.REFUND: self.apply_refund,
            codes.END_OF_SESSION: self.apply_end_of_session,
        }
        handlers[txn.code](txn)

    def apply_create(self, txn):
        """Add a new user unless the name is already taken."""
        if txn.username in self.master.accounts:
            self.log.log_constraint_failure(
                "Duplicate user", f"user '{txn.username}' already exists", txn.raw_line)
            return
        self.master.accounts[txn.username] = UserAccount(
            txn.username, txn.user_type, txn.amount_cents)

    def apply_add_credit(self, txn):
        """Add the transaction's credit amount to an existing user's balance."""
        account = self.master.accounts.get(txn.username)
        if account is None:
            self.log.log_constraint_failure(
                "Unknown user", f"user '{txn.username}' does not exist", txn.raw_line)
            return
        account.credit_cents += txn.amount_cents

    def apply_sell(self, txn):
        """Put a new game up for sale under the seller's name."""
        self.master.listings.append(
            GameListing(txn.game_name, txn.seller, txn.amount_cents))

    def apply_buy(self, txn):
        """Move the price from buyer to seller and add the game to the buyer's collection."""
        buyer = self.master.accounts.get(txn.username)
        seller = self.master.accounts.get(txn.seller)
        if buyer is None or seller is None:
            self.log.log_constraint_failure(
                "Unknown user", "buyer or seller does not exist", txn.raw_line)
            return
        if not buyer.can_afford(txn.amount_cents):
            self.log.log_constraint_failure(
                "Negative balance", f"'{buyer.username}' cannot afford this game", txn.raw_line)
            return
        buyer.credit_cents -= txn.amount_cents
        seller.credit_cents += txn.amount_cents
        self.master.collection.append((buyer.username, txn.game_name))

    def apply_delete(self, txn):
        """Remove a user, cancel their games for sale and drop the games they own."""
        if txn.username not in self.master.accounts:
            self.log.log_constraint_failure(
                "Unknown user", f"user '{txn.username}' does not exist", txn.raw_line)
            return
        del self.master.accounts[txn.username]
        self.master.listings = [g for g in self.master.listings if g.seller != txn.username]
        self.master.collection = [(owner, game) for owner, game in self.master.collection
                                  if owner != txn.username]

    def apply_refund(self, txn):
        """Transfer credit from the seller to the buyer, never leaving the seller negative."""
        buyer = self.master.accounts.get(txn.username)
        seller = self.master.accounts.get(txn.seller)
        if buyer is None or seller is None:
            self.log.log_constraint_failure(
                "Unknown user", "buyer or seller does not exist", txn.raw_line)
            return
        if not seller.can_afford(txn.amount_cents):
            self.log.log_constraint_failure(
                "Negative balance", f"'{seller.username}' cannot cover this refund", txn.raw_line)
            return
        seller.credit_cents -= txn.amount_cents
        buyer.credit_cents += txn.amount_cents

    def apply_end_of_session(self, txn):
        """End-of-session lines mark the end of one Front End run; nothing to update."""
        pass