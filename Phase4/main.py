"""
main.py - Back End (overnight batch processor) for the game distribution service.

Intention:
    Reads the previous day's master files and the merged daily transaction file, applies
    every transaction, and writes the master files used by tomorrow's Front End runs.
    Failed business constraints are printed to the terminal as 'ERROR: <msg>'.

Input files:
    merged transaction file, old accounts file, old available games file, old games collection file
Output files:
    new accounts file, new available games file, new games collection file

How to run (from this folder):
    python main.py <merged_txns> <old_accounts> <old_games> <old_collection> \
                   <new_accounts> <new_games> <new_collection>
"""

import sys

from back_end import BackEnd
from error_log import ErrorLog, FatalError
from master_files import MasterFiles


def main(arguments):
    """Run the nightly batch using the seven file names given on the command line."""
    if len(arguments) != 7:
        print(__doc__)
        return 1
    merged, old_accounts, old_games, old_collection, new_accounts, new_games, new_collection = arguments
    log = ErrorLog()
    try:
        master = MasterFiles()
        master.load(old_accounts, old_games, old_collection)
        BackEnd(master, log).process_file(merged)
        master.save(new_accounts, new_games, new_collection)
    except FatalError as problem:
        print(f"ERROR: Fatal: {problem}")
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
