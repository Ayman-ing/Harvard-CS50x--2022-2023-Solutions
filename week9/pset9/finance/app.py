import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stock = db.execute(
        "select symbol,name,shares,price,shares * price as TOTAL  from stock where user_id=?",
        session["user_id"],
    )
    for dic in stock:
        dic["price"] = usd(dic["price"])
        dic["TOTAL"] = usd(dic["TOTAL"])

    cash = db.execute("select cash from users where id=?", session["user_id"])
    spent = db.execute(
        "select  sum(price * shares) as spent from stock where user_id=?",
        session["user_id"],
    )
    if spent[0]["spent"] == None:
        total = float(cash[0]["cash"])
    else:
        total = float(cash[0]["cash"]) + float(spent[0]["spent"])
    return render_template(
        "index.html", stock=stock, cash=usd(cash[0]["cash"]), total=usd(total)
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # User reached route via POST (as by submitting a form via POST)
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        if (
            (not request.form.get("shares"))
            or (not request.form.get("shares").isnumeric())
            or int(request.form.get("shares")) < 1
        ):
            return apology("give a valid number of shares", 400)
        result = lookup(request.form.get("symbol"))
        if result == None:
            return apology("give a valid symbol", 400)
        amount = db.execute("SELECT cash FROM users where id=?", session["user_id"])
        if amount[0]["cash"] < (result["price"] * int(request.form.get("shares"))):
            return apology("you don't have enough money", 400)
        share = db.execute(
            "select shares from stock where symbol=?", request.form.get("symbol")
        )
        if len(share) == 0:
            db.execute(
                "insert into stock values(?,?,?,CURRENT_TIMESTAMP,?,?)",
                request.form.get("symbol"),
                session["user_id"],
                float(result["price"]),
                int(request.form.get("shares")),
                result["name"],
            )

            db.execute(
                "update users SET cash = cash - ? where id = ?",
                result["price"] * float(request.form.get("shares")),
                session["user_id"],
            )
        else:
            db.execute(
                "update stock SET shares = shares + ? where symbol = ?",
                int(request.form.get("shares")),
                request.form.get("symbol"),
            )
            db.execute(
                "update users SET cash = cash - ? where id = ?",
                result["price"] * float(request.form.get("shares")),
                session["user_id"],
            )
        db.execute(
            "insert into history values(?,?,?,CURRENT_TIMESTAMP,?,?,'buy')",
            request.form.get("symbol"),
            session["user_id"],
            float(result["price"]),
            int(request.form.get("shares")),
            result["name"],
        )
        return redirect("/")

    else:
        # User reached route via GET
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute(
        "select name,price,trans_at,shares,trans_type from history where user_id=?",
        session["user_id"],
    )
    for dic in history:
        dic["price"] = usd(dic["price"])

    return render_template("history.html", history=history)


@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    # User reached route via POST (as by submitting a form via POST)

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("current_password"):
            return apology("must provide current username", 403)

        # Ensure password was submitted
        if not request.form.get("new_password"):
            return apology("must provide new password", 403)
        if not request.form.get("confirmation"):
            return apology("must confirm new password", 403)
        if request.form.get("confirmation") != request.form.get("new_password"):
            return apology("confirmation must match password", 403)

        # Query database for password
        rows = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        # Ensure  password is correct
        if not check_password_hash(
            rows[0]["hash"], request.form.get("current_password")
        ):
            return apology("password incorrect", 403)
        newpw = generate_password_hash(request.form.get("new_password"))
        db.execute("update users set hash=? where id=?", newpw, session["user_id"])

        # Redirect user to home page

        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change.html")


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
    if request.method == "POST":
        """Get stock quote."""
        if not request.form.get("symbol"):
            return apology("give a stock symbol", 400)
        result = lookup(request.form.get("symbol"))
        if result == None:
            return apology("symbol doesn't exist", 400)
        result["price"] = usd(result["price"])
        return render_template("quoted.html", result=result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)
        # Ensure password confirmation match the password
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("password must much confirmation", 400)
        # query the database
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        # checking the username is unique
        if len(rows) > 0:
            return apology("username taken ", 400)
        # generating the hash password
        pwhash = generate_password_hash(request.form.get("password"))
        # adding the user
        db.execute(
            "INSERT INTO users (username,hash) VALUES (?,?)",
            request.form.get("username"),
            pwhash,
        )
        # getting the user id
        user_id = db.execute(
            "SELECT id FROM users WHERE username = ?", request.form.get("username")
        )
        # Remember which user has logged in
        session["user_id"] = user_id[0]["id"]

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
        if request.form.get("symbol") == "":
            return apology("must choose symbol")
        if not request.form.get("shares"):
            return apology("must provide number of shares")
        if (not request.form.get("shares").isnumeric()) or int(
            request.form.get("shares")
        ) < 0:
            return apology("give a valid number of shares")

        # bought=db.execute("select shares from stock where symbol= ? and user_id = ? and trans_type='buy'",request.form.get("symbol"),session["user_id"])
        # sold=db.execute("select shares from stock where symbol= ? and user_id = ? and trans_type='sell'",request.form.get("symbol"),session["user_id"])
        # shares = int(bought[0]["sahres"])-int(sold[0]["sahres"])
        shares = db.execute(
            "select shares from stock where symbol= ? and user_id = ?",
            request.form.get("symbol"),
            session["user_id"],
        )
        if int(shares[0]["shares"]) == 0:
            return apology("you don't have this stock")

        if int(shares[0]["shares"]) < int(request.form.get("shares")):
            return apology("not enough shares")
        # result=lookup(request.form.get("symbol"))
        price = db.execute(
            "select price from stock where symbol=? and user_id=?",
            request.form.get("symbol"),
            session["user_id"],
        )
        db.execute(
            "update users set cash = cash + ? where id=?",
            float(price[0]["price"]) * float(request.form.get("shares")),
            session["user_id"],
        )
        # db.execute("update users set cash = cash + ? where id=?",float(result["price"])*float(request.form.get("shares")),session["user_id"])
        result = lookup(request.form.get("symbol"))
        if int(shares[0]["shares"]) == int(request.form.get("shares")):
            db.execute(
                "delete from stock where symbol=? and user_id=?",
                request.form.get("symbol"),
                session["user_id"],
            )

        else:
            db.execute(
                "update stock set shares= shares - ? where symbol=? and user_id=?",
                request.form.get("shares"),
                request.form.get("symbol"),
                session["user_id"],
            )
        db.execute(
            "insert into history values(?,?,?,CURRENT_TIMESTAMP,?,?,'sell')",
            request.form.get("symbol"),
            session["user_id"],
            float(price[0]["price"]),
            int(request.form.get("shares")),
            result["name"],
        )

        return redirect("/")

    else:
        stock = db.execute(
            "select symbol from stock where user_id=?", session["user_id"]
        )

        return render_template("sell.html", stock=stock)
