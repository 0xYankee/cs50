import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Retrieve user's stocks from the database
    stocks = db.execute(
        "SELECT symbol FROM stocks WHERE user_id = ? AND transaction_type = 'buy' GROUP BY symbol", session["user_id"])

    portfolio = []

    for stock in stocks:
        symbol = stock["symbol"]
        transactions = db.execute(
            "SELECT shares, remaining_shares, price, transaction_type FROM stocks WHERE user_id = ? AND symbol = ? AND transaction_type = 'buy' AND remaining_shares != 0", session["user_id"], symbol)

        if len(transactions) != 0:
            # Calculate total shares
            remaining_shares = sum(transaction["remaining_shares"] for transaction in transactions)

            # Calculate total price
            total_price = usd(sum(transaction["price"] for transaction in transactions))

            # Calculate average price
            average_price = sum(transaction["price"] for transaction in transactions) / len(transactions)

            # Fetch current price using an API or another data source
            current_data = lookup(symbol)
            current_price = current_data["price"]

            # Calculate profit or loss
            profit_loss = (current_price - average_price) * remaining_shares
            current_price = usd(current_price)
            average_price = usd(average_price)

            portfolio.append({
                "symbol": symbol,
                "shares": remaining_shares,
                "price": total_price,
                "average_price": average_price,
                "current_price": current_price,
                "profit_loss": profit_loss
            })

    # Retrieve user's remaining cash from the database
    remaining_cash = usd(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])

    # Check if the purchase was successful
    purchase_success = request.args.get("purchase_success")
    stock_symbol = request.args.get("symbol")
    total_cost = request.args.get("total_cost")
    if purchase_success:
        # Render the index template with the success banner
        return render_template("index.html", portfolio=portfolio, remaining_cash=remaining_cash, purchase_success=purchase_success, stock_symbol=stock_symbol, total_cost=total_cost)

    sell_success = request.args.get("sell_success")
    sell_symbol = request.args.get("sell_symbol")
    total_pnl = request.args.get("total_pnl")
    if sell_success:
        # Render the index template with the success banner
        return render_template("index.html", portfolio=portfolio, remaining_cash=remaining_cash, sell_success=sell_success, sell_symbol=sell_symbol, total_pnl=total_pnl)

    # Render the index template without the success banner
    return render_template("index.html", portfolio=portfolio, remaining_cash=remaining_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol and shares were submitted
        symbol = request.form.get("symbol")
        shares_to_buy = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        if not shares_to_buy:
            return apology("must provide shares", 400)

        if not shares_to_buy.isdigit():
            return apology("invalid amount of shares")

        # Call the lookup function and check if stock exists
        stock_info = lookup(symbol)
        if stock_info is None:
            return apology("invalid stock", 400)

        name = stock_info["name"]
        price = float(stock_info["price"])
        symbol = stock_info["symbol"]

        # Check if shares is a positive integer
        if int(shares_to_buy) < 1:
            return apology("invalid amount of shares", 400)

        # Check if user has enough cash to purchase the shares
        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = float(user[0]["cash"])
        total_cost = int(shares_to_buy) * price
        if total_cost > cash:
            return apology("insufficient cash to purchase shares", 400)

        # Update user's cash
        updated_cash = cash - total_cost
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])

        # Insert the bought stocks into the database
        db.execute("INSERT INTO stocks (timestamp, user_id, symbol, price, shares, remaining_shares, transaction_type) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   datetime.now(), session["user_id"], symbol.upper(), price, shares_to_buy, shares_to_buy, "buy")
        total_cost = usd(total_cost)
        updated_cash = usd(updated_cash)
        purchase_success = True

        return redirect(url_for("index", name=name, total_cost=total_cost, symbol=symbol, purchase_success=purchase_success))

    else:
        # Retrieve user's cash amount from the database
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        return render_template("buy.html", cash=cash)


@app.route("/history")
@login_required
def history():
    """Display transaction history"""

    # Retrieve transaction history from the database
    transactions = db.execute(
        "SELECT transaction_type, symbol, shares, price, timestamp FROM stocks WHERE user_id = ? ORDER BY timestamp DESC", session["user_id"])

    # Render the history template with the transaction data
    return render_template("history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Call the lookup function and get quotes and return the quotes to quoted.html to display
        infos = lookup(request.form.get("symbol"))

        if infos is None:
            return apology("ticker not found", 400)

        name = infos["name"]
        price = usd(float(infos["price"]))
        symbol = infos["symbol"]

        return render_template("quoted.html", name=name, price=price, symbol=symbol)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation of password", 400)

        # Ensure password and confirmation is identical
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("password does not match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username does not exist
        if len(rows) > 0:
            return apology("username already exists", 400)

        # Insert new username and hash to db
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol and shares were submitted
        sell_symbol = request.form.get("symbol").upper()
        shares_to_sell = float(request.form.get("shares"))

        if not sell_symbol:
            return apology("must provide symbol", 400)

        if not shares_to_sell:
            return apology("must provide shares", 400)

        # Select from database if user owns this stock
        stocks = db.execute(
            "SELECT id, shares, price, remaining_shares FROM stocks WHERE symbol = ? AND user_id = ? AND transaction_type = 'buy'", sell_symbol, session["user_id"])

        if len(stocks) < 1:
            return apology("you do not own this stock", 400)

        # Check if shares is a valid number
        total_shares = sum(stock["remaining_shares"] for stock in stocks)
        if shares_to_sell < 1 or shares_to_sell > total_shares:
            return apology("invalid number of shares", 400)

        # Call the lookup function and check current price
        stock_info = lookup(sell_symbol)
        if stock_info is None:
            return apology("invalid, please try again", 400)

        current_price = float(stock_info["price"])

        # Sell stocks and calculate profit or loss
        total_pnl = 0
        for stock in stocks:
            stock_id = stock["id"]
            stock_shares = stock["remaining_shares"]
            stock_price = float(stock["price"])
            if stock_shares > shares_to_sell:
                pnl = current_price * shares_to_sell
                db.execute("UPDATE stocks SET remaining_shares = remaining_shares - ? WHERE id = ?", shares_to_sell, stock_id)
                db.execute("INSERT INTO stocks (timestamp, user_id, symbol, price, shares, remaining_shares, transaction_type) VALUES(?, ?, ?, ?, ?, ?, ?)",
                           datetime.now(), session["user_id"], sell_symbol.upper(), stock_price, shares_to_sell, 0, "sell")
                total_pnl += pnl
                break
            else:
                pnl = current_price * stock_shares
                db.execute("UPDATE stocks SET remaining_shares = remaining_shares - ? WHERE id = ?", shares_to_sell, stock_id)
                db.execute("INSERT INTO stocks (timestamp, user_id, symbol, price, shares, remaining_shares, transaction_type) VALUES(?, ?, ?, ?, ?, ?, ?)",
                           datetime.now(), session["user_id"], sell_symbol.upper(), stock_price, stock_shares, 0, "sell")
                total_pnl += pnl
                shares_to_sell -= stock_shares

        # Set new cash amount based on profit or loss
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + total_pnl, user_id)
        total_pnl = usd(total_pnl)
        sell_success = True

        return redirect(url_for("index", total_pnl=total_pnl, sell_symbol=sell_symbol, sell_success=sell_success))

    else:
        # User reached route via GET, display the sell form
        # Select stocks owned by the user to populate the select field
        user_id = session["user_id"]
        stocks = db.execute("SELECT symbol FROM stocks WHERE user_id = ? AND transaction_type = 'buy' GROUP BY symbol", user_id)
        return render_template("sell.html", stocks=stocks)


@app.route("/topup", methods=["GET", "POST"])
@login_required
def topup():
    """Top up cash"""

    # Ensure topup amount was submitted
    if request.method == "POST":

        if not request.form.get("topup"):
            return apology("must provide number", 400)

        try:
            topup = float(request.form.get("topup"))
            if topup < 1:
                return apology("invalid amount", 400)
        except ValueError:
            return apology("invalid amount", 400)

        # Retrieve user's cash amount from the database
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Update user's cash amount with the topup
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + topup, user_id)

        return redirect("/")

    else:
        # Retrieve user's cash amount from the database
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        return render_template("topup.html", cash=cash)
