import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.utils import secure_filename
from flask_ckeditor import CKEditor

from helpers import login_required, convert_date, apology


# Configure application
app = Flask(__name__)
# Add CKEditor
app.config['CKEDITOR_PKG_TYPE'] = 'basic'
ckeditor  = CKEditor(app)


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///resume.db")

# A list to stores the file name of uploaded file in a session to aid with default image loading.
uploadedimage = []


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    return redirect("/mkr")


@app.route("/mkr", methods=["GET", "POST"])
@login_required

def saveform():

        photoname = 'photo'+ str(session["user_id"])

        if request.method == "POST":

            user_id = session["user_id"]
            name = request.form.get("name")
            job_title = request.form.get("job_title")
            phone = request.form.get("phone")
            email = request.form.get("email")
            linkdin = request.form.get("linkdin")
            profile = request.form.get('profile')
            theme = request.form.get('selectedTheme')
            skill1 = request.form.get('skill1')
            skill2 = request.form.get('skill2')
            skill3 = request.form.get('skill3')
            specialtitle = request.form.get('specialtitle')
            specialitem1 = request.form.get('specialitem1')
            specialitem2 = request.form.get('specialitem2')
            course1 = request.form.get('course1')
            institution1 = request.form.get('institution1')
            title1 = request.form.get('jobtitle1input1')
            company_name1 = request.form.get('companyname1')
            starting_date1 = request.form.get('startingdate1')
            ending_date1 = request.form.get('endingdate1')
            job_discription1 = request.form.get('jobDiscription1input')

            # Check if user data exists
            existing_data = db.execute("SELECT * FROM details WHERE user_id = ?", user_id)

            if existing_data:
                # Perform UPDATE
                db.execute("""
                    UPDATE details
                    SET name = ?, job_title = ?, phone = ?, email = ?, linkdin = ?, profile = ?, theme = ?, skill1 = ?, skill2 = ?, skill3 = ?, specialtitle = ?, specialitem1 = ?, specialitem2 = ?, course1 = ?, institution1 = ?, title1 = ?, company_name1 = ?, starting_date1 = ?, ending_date1 = ?, job_discription1 = ?
                    WHERE user_id = ?
                """, name, job_title, phone, email, linkdin, profile, theme, skill1, skill2, skill3, specialtitle, specialitem1, specialitem2, course1, institution1, title1, company_name1, starting_date1, ending_date1, job_discription1, user_id)
            else:
                # Perform INSERT
                db.execute("""
                    INSERT INTO details
                    (user_id, name, job_title, phone, email, linkdin, profile, theme, skill1, skill2, skill3, specialtitle, specialitem1, specialitem2, course1, institution1, title1, company_name1, starting_date1, ending_date1, job_discription1)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                """, user_id, name, job_title, phone, email, linkdin, profile, theme, skill1, skill2, skill3, specialtitle, specialitem1, specialitem2, course1, institution1, title1, company_name1, starting_date1, ending_date1, job_discription1)



        # Rendering the Web Page, going to GET route
            return redirect('/mkr')

        else:
            # when user reaches mkr via GET, if he already has database he will get his details loaded
            # else we make a empty dictionary and pass it render_template, where we aready have logic for dealing with no values. (showing default)
            try:
                user_details = db.execute("SELECT * FROM details WHERE user_id = ?", session["user_id"])[0]
            except IndexError:
                user_details = {}

            print(user_details)

            return render_template("maker.html", image=photoname, user_details=user_details, uploadedimage=uploadedimage, convert_date=convert_date)

@app.route("/mkrimgupload", methods=["POST"])
@login_required

def upload_image():

        if request.method == "POST":
            uploaded_image = request.files['file']
            if uploaded_image.filename != '':

                photoname = 'photo'+ str(session["user_id"])
                file_path = os.path.join("/workspaces/132731110/finalproject/resumeapp/static/photos", photoname)
                uploaded_image.save(file_path)

                uploadedimage.append(photoname)

                return redirect("/mkr")



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
        return redirect("/mkr")

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

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # When user submits registration form via POST
    if request.method == "POST":
        if not  request.form.get("username"):
            return apology("must enter username", 400)
        if not  request.form.get("password"):
            return apology("must enter password", 400)
        if not  request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Search for an existing user with the same username
        row = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(row) != 0:
            return apology("username already taken", 400)
        username = request.form.get("username")

        # Check password and confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password doesn't match", 400)
        hash = generate_password_hash(request.form.get("password"), method='pbkdf2', salt_length=16)

        # if eveything is correct add to database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        return redirect("/")

    # When user reaches the register page via GET
    else:
        return render_template("register.html" )
