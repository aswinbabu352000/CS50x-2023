import csv
from datetime import datetime
import pytz
import requests
import subprocess
import urllib
import uuid

from flask import redirect, render_template, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def convert_date(input_date):
    """Convert date"""
    try:
        # Parse the input date string
        parsed_date = datetime.strptime(input_date, '%Y-%m')

        # Format the date as 'Month YYYY'
        formatted_date = parsed_date.strftime('%b %Y')

        return formatted_date
    except ValueError:
        # Handle invalid date format
        # return 'Invalid Date Format'
        return 'Present'
        # Hack to get to show Present when no date is given, should change later



