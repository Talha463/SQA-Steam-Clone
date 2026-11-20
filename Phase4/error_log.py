"""
error_log.py

Prints Back End errors to the terminal in the required form 'ERROR: <msg>'.
"""

import sys


class FatalError(Exception):
    """Raised when a file is unreadable or corrupt; the Back End must stop."""


class ErrorLog:
    """Writes failed-constraint and fatal errors to the terminal."""

    def log_constraint_failure(self, error_type, description, transaction_line):
        """Report a transaction that broke a business constraint (processing continues)."""
        print(f"ERROR: {error_type}: {description} [transaction: {transaction_line}]")

    def log_fatal(self, error_type, description, filename):
        """Report an unrecoverable error and the file that caused it."""
        print(f"ERROR: {error_type}: {description} [file: {filename}]", file=sys.stderr)
