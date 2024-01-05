import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addCash():
    """Add money to existing balance"""

    if request.method == "POST":
        current_cash = int(
            db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
                "cash"
            ]
        )
        cash_to_add = int(request.form.get("addcash"))
        if cash_to_add > 0:
            final_cash = current_cash + cash_to_add
        else:
            apology("please input a positive integer")

        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", final_cash, session["user_id"]
        )
        return redirect("/")

    else:
        current_cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )[0]["cash"]
        return render_template("addcash.html", cash=current_cash, usd=usd)


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get users portfolio (details of thier stock owned)
    owned = db.execute("SELECT * FROM owned WHERE user_id = ?", session["user_id"])

    cash_in_stocks = 0
    for row in owned:
        cash_in_stocks = cash_in_stocks + (
            lookup(row["symbol"])["price"] * row["shares"]
        )

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
        "cash"
    ]

    total_cash = usd(cash_in_stocks + cash)

    return render_template(
        "index.html", owned=owned, lookup=lookup, cash=cash, total=total_cash, usd=usd
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reaches via POST, i.e, clicked buy button.
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        if lookup(request.form.get("symbol")) == None:
            return apology("stock dosent exist. check symbol.", 400)

        if not request.form.get("shares").isnumeric():
            return apology("please input a positive integer")

        api_result = lookup(request.form.get("symbol"))
        user_id = session["user_id"]
        no_of_shares = int(request.form.get("shares"))
        symbol = api_result["symbol"]
        current_price = float(api_result["price"])
        total_amount = current_price * no_of_shares
        name = api_result["name"]

        cash_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0][
            "cash"
        ]

        if total_amount > cash_balance:
            return apology("you don't have enough money", 400)

        cash_balance -= total_amount

        # If eveything is correct / shares bought
        # add bought stocks to user's portfolio (owned)

        # if buying stock of company for first time
        row = db.execute(
            "SELECT * FROM owned WHERE user_id = ? AND symbol = ?", user_id, symbol
        )
        if len(row) == 0:
            db.execute(
                "INSERT INTO owned ('user_id', 'symbol', 'shares', 'name') VALUES (?,?,?,?)",
                user_id,
                symbol,
                no_of_shares,
                name,
            )
        else:
            current_shares = db.execute(
                "SELECT shares FROM owned WHERE user_id = ? AND symbol = ?",
                user_id,
                symbol,
            )[0]["shares"]
            new_shares = current_shares + no_of_shares
            db.execute(
                "UPDATE owned SET shares = ? WHERE user_id = ? AND symbol = ?",
                new_shares,
                user_id,
                symbol,
            )

        # enter transaction into transcations
        db.execute(
            "INSERT INTO trasnactions (user_id, symbol, shares, trnsprice, type) VALUES (?, ?, ?, ?, ?)",
            user_id,
            symbol,
            no_of_shares,
            current_price,
            "b",
        )

        # update cash balance of user
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_balance, user_id)

        return redirect("/")

    # User reaches via GET(E.g. Clicks 'Buy' from Navbar).
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute(
        "SELECT * FROM trasnactions WHERE user_id = ?",
        user_id,
    )

    return render_template("history.html", usd=usd, transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not lookup(request.form.get("symbol")) == None:
            result = lookup(request.form.get("symbol"))
        else:
            return apology("invalid symbol", 400)
        return render_template("quoted.html", result=result, usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # When user submits registration form via POST
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must enter username", 400)
        if not request.form.get("password"):
            return apology("must enter password", 400)
        if not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Search for an existing user with the same username
        row = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        if len(row) != 0:
            return apology("username already taken", 400)
        username = request.form.get("username")

        # Check password and confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password doesn't match", 400)
        hash = generate_password_hash(
            request.form.get("password"), method="pbkdf2", salt_length=16
        )

        # if eveything is correct add to database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        return redirect("/")

    # When user reaches the register page via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # user submit the sell form
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("must provide stock symbol and shares", 400)

        if not request.form.get("shares").isnumeric():
            return apology("enter a positive integer", 400)

        no_of_shares = int(request.form.get("shares"))
        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        shares_of_stock = db.execute(
            "SELECT * FROM owned WHERE user_id = ? AND symbol = ?", user_id, symbol
        )
        shares_owned = int((shares_of_stock)[0]["shares"])
        current_price = float(lookup(symbol)["price"])
        total_amount = current_price * no_of_shares
        current_balance = float(
            db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        )

        if no_of_shares > shares_owned:
            return apology("you dont own that many shares", 400)
        if no_of_shares == 0:
            return apology("enter a positive integer", 400)

        # enter transaction into transcations
        db.execute(
            "INSERT INTO trasnactions (user_id, symbol, shares, trnsprice, type) VALUES (?, ?, ?, ?, ?)",
            user_id,
            symbol,
            no_of_shares,
            current_price,
            "s",
        )

        # remove shares from user's stock
        current_shares_of_symbol = shares_owned - no_of_shares
        db.execute(
            "UPDATE owned SET shares = ? WHERE user_id = ? AND symbol = ?",
            current_shares_of_symbol,
            user_id,
            symbol,
        )

        # remove row if user has no more shares of that symbol after selling
        if current_shares_of_symbol == 0:
            db.execute(
                "DELETE FROM owned WHERE user_id = ? AND symbol = ?", user_id, symbol
            )

        # update cash balance of user

        current_balance += total_amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", current_balance, user_id)

        return redirect("/")  # $192,449.00

    else:
        total_stocks_owned = db.execute(
            "SELECT * FROM owned WHERE user_id = ?", session["user_id"]
        )
        return render_template("sell.html", owned=total_stocks_owned)
