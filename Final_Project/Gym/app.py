import os
# pylint: disable=E0611

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session , url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from flask import redirect, render_template, session
from functools import wraps
from flask_moment import Moment
import datetime
import calendar
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


# Configure application
app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)
moment = Moment(app)
# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///gym1.db")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return render_template("login.html",message1="must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return render_template("login.html",message2="must provide password")
        # Query database for username
        rows = db.execute(
            "SELECT ad_id,hash FROM Admin WHERE ad_username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return render_template("login.html",message3="username or password incorrect")

        # Remember which user has logged in
        session["user_id"] = rows[0]["ad_id"]

        # Redirect user to home page

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return render_template("register.html", message1="must provide username")
        if not request.form.get("mail"):
            return render_template("register.html", message2="must provide your mail")
        # Ensure password was submitted
        elif not request.form.get("password"):
            return render_template("register.html", message3="must provide password")
        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return render_template("register.html", message4="must provide confirmation")
        # Ensure password confirmation match the password
        elif request.form.get("confirmation") != request.form.get("password"):
            return render_template("register.html", message5="password must match confirmation")
        # query the database
        rows_username = db.execute(
            "SELECT * FROM Admin WHERE ad_username = ?", request.form.get("username")
        )
        rows_mail = db.execute("SELECT * FROM Admin WHERE ad_mail = ?", request.form.get("mail"))
        # checking the username is unique
        if len(rows_username) > 0:
            return  render_template("register.html", message6="username taken")
        #checking the mail doesn't exist
        if len(rows_mail) > 0:
            return  render_template("register.html", message7="mail already exist")
        # generating the hash password
        pwhash = generate_password_hash(request.form.get("password"))
        # adding the user
        db.execute(
            "INSERT INTO Admin (ad_username,ad_mail,hash) VALUES (?,?,?)",
            request.form.get("username"),
            request.form.get("mail"),
            pwhash

        )
        # getting the user id
        user_id = db.execute(
            "SELECT ad_id FROM Admin WHERE ad_username = ?", request.form.get("username")
        )
        # Remember which user has logged in
        session["user_id"] = user_id[0]["ad_id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
@login_required
@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route("/members", methods=["GET", "POST"])
@login_required
def members():
    #the delete function
    def delete():
        #checking if it's deleting a member or a membership
        if request.form.get("form_id")=="form3cl" :
            db.execute("delete from client where cl_id=?",int(request.form.get("cl_id")))
            return render_template("members.html",message8="member deleted succssefuly",member=member,moment=moment,clients=clients,sport=sport)
        else:
            db.execute("delete from membership where cl_id=?",int(request.form.get("cl_id")))
            return render_template("members.html",message8="membership deleted succssefuly",member=member,moment=moment,clients=clients,sport=sport)

    #the edit function
    def edit():
        #the client editing function
        def edit_cl():
            if request.form.get("cl_num"):   
                #checking the phone number already exist
                num=db.execute("select cl_Fname from client where cl_num=?;",request.form.get("cl_num"))
                if len(num)>0:
                    return render_template('members.html', message3="phone number already exist",member=member,moment=moment,clients=clients,sport=sport)
                #checking the validty of the phone number
                if (not request.form.get("cl_num").isnumeric()) or len(request.form.get("cl_num"))<8:
                    return render_template('members.html', message1="phone number invalid",member=member,moment=moment,clients=clients,sport=sport)
            dict_m={"cl_Fname":request.form.get("cl_Fname"),"cl_Lname":request.form.get("cl_Lname"),"cl_BD":request.form.get("cl_BD"),
                    "cl_num":request.form.get("cl_num"),"student":request.form.get("student")}
            for key,value in dict_m.items():
                if value:
                    db.execute("UPDATE client set ?=? where cl_id=?",key,value,int(request.form.get("cl_id")))
        #the membership editing function    
        def edit_m():
            result=edit_cl()
        
            if result is None :
                dict_msh={"m_type":request.form.get("m_type"),"duration":request.form.get("duration"),"price":request.form.get("price")}
                
                #"sp_name"
                if request.form.get("sp_name"):
                    sp_id=db.execute("select sp_id from sport where sp_name=?",request.form.get("sp_name"))
                    db.execute("update membership set sp_id=? where m_id=?",sp_id[0]["sp_id"],int(request.form.get("m_id")))
                    
                if request.form.get("starts_at") or request.form.get("duration"):
                    db.execute("UPDATE membership set starts_at=? ,ends_at=date(?,'+' || ? || ' months') where m_id=?",request.form.get("starts_at"),request.form.get("starts_at"),int(request.form.get("duration")),int(request.form.get("m_id")))
                
                for key,value in dict_msh.items():
                    if value:
                        db.execute("UPDATE membership set ?=? where m_id=?",key,value,int(request.form.get("m_id")))
                return render_template("members.html",message7="changes made successfuly",member=member,moment=moment,clients=clients,sport=sport)
            else :
                return result       
        #checking whether it's editing a client or a client and membership
        if request.form.get("form_id") =="form2m":
            return edit_m()
        elif request.form.get("form_id") =="form2cl":
            result2=edit_cl()
            if result2 is None:
                return render_template("members.html",message7="changes made successfuly",member=member,moment=moment,clients=clients,sport=sport)
            else :
                return result2
    #add function
    def add():
        #adding client
        def add_cl():
            #checking the validty of the phone number
            if (not request.form.get("cl_num").isnumeric()) or len(request.form.get("cl_num"))<8:
                return render_template('members.html', message1="phone number invalid",member=member,moment=moment,clients=clients,sport=sport)
            #checking the phone number already exist
            number=db.execute("select cl_Fname from client where cl_num=?;",request.form.get("cl_num"))
            if len(number)>0:
                return render_template('members.html', message3="phone number already exist",member=member,moment=moment,clients=clients,sport=sport)
            #adding the member to the table
            db.execute(
                    "insert into client (cl_Fname,cl_Lname,cl_num,cl_BD,student) values(?,?,?,?,?)",
                    request.form.get("cl_Fname"),
                    request.form.get("cl_Lname"),
                    int(request.form.get("cl_num")),
                    request.form.get("cl_BD"),
                    request.form.get("student") 

                )
        #adding membership
        def add_m():
            
            #adding the membership
            if (not request.form.get("sp_name")) and (not request.form.get("m_type"))  and  (not request.form.get("duration")) and (not request.form.get("price")) and (not request.form.get("starts_at")):    
                return "member added successfuly"
            if (not request.form.get("sp_name")) or (not request.form.get("m_type"))  or  (not request.form.get("duration")) or (not request.form.get("price")) or (not request.form.get("starts_at")):
                return "No membership added ! Not enough informations"
            if not request.form.get("cl_id"):
                cl_id=db.execute("select cl_id from client where cl_num=?",int(request.form.get("cl_num")))
            else:
                cl_id=[{}]
                cl_id[0]["cl_id"]=int(request.form.get("cl_id"))
            sp_id=db.execute("select sp_id from sport where sp_name=?",request.form.get("sp_name"))
            db.execute(
                    "insert into membership (cl_id,sp_id,m_type,duration,price,starts_at,ends_at) values(?,?,?,?,?,?,date(?,'+' || ? || ' months'))",
                    cl_id[0]["cl_id"],
                    sp_id[0]["sp_id"],
                    request.form.get("m_type"),
                    int(request.form.get("duration")),
                    int(request.form.get("price")),
                    request.form.get("starts_at"),
                    request.form.get("starts_at"),
                    int(request.form.get("duration")),
                    
                    )
            
        #checking whether it's adding a membership only or  all full information
        if request.form.get("form_id") =="form1cl":
            result1=add_m()
            if result1 is None:

                return render_template('members.html',message5="membership added successfuly",member=member,moment=moment,clients=clients,sport=sport)
            else:
                return render_template("members.html",message3="No membership added ! Not enough informations",member=member,moment=moment,clients=clients,sport=sport)
        elif request.form.get("form_id") =="form1m": 
            result2=add_cl()
            if result2 is None :
                result3=add_m()
                if result3 is None:
                    return render_template('members.html',message5="member and his membership added successfuly",member=member,moment=moment,clients=clients,sport=sport)
                else:
                    return render_template('members.html',message5="member added successfuly",message3="No membership added ! Not enough informations",member=member,moment=moment,clients=clients,sport=sport)
            else:
                return result2
    
    
    
    moment = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    #getting necessary informations 
    sport=db.execute("select sp_name from sport;")
    member=db.execute("select * from member;")
    clients=db.execute("select cl_id,cl_Fname,cl_Lname,cl_num,cl_BD,student from client where cl_id NOT IN (SELECT cl_id from membership) ;")
    for m in member:
        if moment  >= m["ends_at"]:
            m["status"]="inactive"
        else:
            m["status"]="active"
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        #checking if it's an adding,edit or a delete
        if "form1" in request.form.get("form_id"):
            return add()
        elif "form2" in request.form.get("form_id") :
            return edit()
        elif "form3" in request.form.get("form_id") :
            return delete()
            
        
        
        
    #user reached route via GET    
    else:
            
        return render_template('members.html',member=member,moment=moment,clients=clients,sport=sport)
def generate_calendar(year, month):
        # Create a calendar for the specified year and month
        cal = calendar.monthcalendar(year, month)
        return cal
@app.route("/schedule/<int:year>/<int:month>", methods=["GET", "POST"])
@login_required
def schedule(year,month):
    def add(days):
        

        #taking the sport id's from their names 
        sp_id=db.execute("select sp_id from sport where sp_name=?",request.form.get("sp_name"))
        
        if request.form.get("repeat_pattern")=="irregular":
            #inserting the event only in the day clicked
            db.execute("insert into schedule  (sp_id,starts_at,ends_at,repeat_pattern,days,start_date,end_date) values(?,?,?,?,?,?,?)",
                    sp_id[0]["sp_id"],
                    request.form.get("starts_at"),
                    request.form.get("ends_at"),
                    request.form.get("repeat_pattern"),
                    days,
                    request.form.get("event_date"),request.form.get("event_date"),
                                        )
        #inserting the event
        else:
            db.execute("insert into schedule  (sp_id,starts_at,ends_at,repeat_pattern,days,start_date,end_date) values(?,?,?,?,?,?,?)",
                        sp_id[0]["sp_id"],
                        request.form.get("starts_at"),
                        request.form.get("ends_at"),
                        request.form.get("repeat_pattern"),
                        days,
                        request.form.get("start_date"),
                        request.form.get("end_date"),
                                            )
    def edit(days):
        
        #putting the list of the days in a string
       
        dict_event={"sp_id":request.form.get("sp_id"),"starts_at":request.form.get("starts_at"),
                    "end_at":request.form.get("ends_at"),"repeat_pattern":request.form.get("repeat_pattern"),
                    "days":days,"start_date":request.form.get("start_date"),"end_date":request.form.get("end_date")
        }
        for key,value in dict_event.items():
            if value:
                db.execute("update schedule set ?=? where sc_id=?;",key,value,request.form.get("sc_id"))
    def delete():
        db.execute("delete from schedule where sc_id=?",request.form.get("sc_id"))
    
    if request.method =="GET":
        #getting the month calendar
        cal=generate_calendar(year,month)
        #getting the events 
        events=db.execute("select * from schedule;")
        current_events={}
        #iterating for each event in this month
        for activity in events:
            activity["sp_name"]=db.execute("select sp_name from sport where sp_id=?",activity["sp_id"])
            if activity["repeat_pattern"]!="irregular":
                #getting the sport name from the id
                
                #setting the condtions to display the events in the correct dates
                start_month_condition=(int(activity["start_date"][0:4])==year and int(activity["start_date"][5:7])<=month)
                start_year_condtion=int(activity["start_date"][0:4])<year
                end_month_condition=(int(activity["end_date"][0:4])==year and int(activity["end_date"][5:7])>=month)
                end_year_condition=int(activity["end_date"][0:4])>year
                #checking for the events that are in that month 
                if (start_year_condtion or start_month_condition   )and (end_year_condition or end_month_condition ):
                    #making the list of the days that event appear
                    days=[day for day in activity["days"].split(",") if day]
                    #iterating for every week
                    for week in cal:
                        #iterating for every day
                        for day in range(7):
                            #checking for days that match the repeat pattern
                            if str(day) in days and week[day]!=0 :
                                #checking if we are before the start day if it's the same year and month (that's why we put not)
                                start_day_condition=(int(activity["start_date"][0:4])==year and int(activity["start_date"][5:7])==month  and week[day]<int(activity["start_date"][8:]))
                                #checking that we are after the end day if it's the same year and month (that's why we put not)
                                end_day_condition=(int(activity["end_date"][0:4])==year and int(activity["end_date"][5:7])==month  and week[day]>int(activity["start_date"][8:]))
                                if not (start_day_condition) and not (end_day_condition ):
                                    #putting the event date and info in the dictionarry
                                    formatted_date = '{:04d}-{:02d}-{:02d}'.format(year, month,week[day])
                                    current_events[str(activity["sc_id"])+formatted_date]=[formatted_date,activity["sp_name"][0]["sp_name"],activity["starts_at"],activity["ends_at"]]
            else:
                #for the irregular events
                current_events[str(activity["sc_id"])+activity["start_date"]]=[activity["start_date"],activity["sp_name"][0]["sp_name"],activity["starts_at"],activity["ends_at"]]
            #changing the numbers for their correspond days name
            week_days=["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"]
        
            days_output=""
            if activity["days"]:
                for i in range(7):
                    if str(i) in activity["days"]:
                        days_output=days_output + week_days[i] + " "
                activity["days_output"]=days_output
            
        
        
        #taking all the sport for the form
        sport=db.execute("select sp_id,sp_name from sport;")
        return render_template("schedule.html",events=events,cal=cal,start_month_condition=start_month_condition,month=month,year=year,current_events=current_events,sport=sport,week_days=week_days)
    
    
    
    #POST 
    elif request.method=="POST":
        if request.form.get("repeat_pattern")!="irregular":
            days=""
            list=request.form.getlist("days[]")
        
        #putting the list of the days in a string
            for day in list:
                days=days + day +","
        else:
            days=""
        if request.form.get("form_id")=="formsch-add":
            add(days)

        elif request.form.get("form_id")=="formsch-edit":
            edit(days)
        elif request.form.get("form_id")=="formsch-delete":
            delete()
        
        
        date=request.form.get("event_date")
        events=db.execute("select * from schedule;")
        #changing the numbers for their correspond days name
        week_days=["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"]
        for event in events:
            days_output=""
            if event["days"]:
                for i in range(7):
                    if str(i) in event["days"]:
                        days_output=days_output + week_days[i] + " "
            event["days"]=days_output
            event["sp_name"]=db.execute("select sp_name from sport where sp_id=?",event["sp_id"])
        return render_template("schedule.html",success="Success ! refresh page to see result",year=year,month=month,date=date,days=days,events=events,week_days=week_days)

@app.route('/schedule', methods=["GET", "POST"])
@login_required
def current_month_calendar():
    if request.method =="GET":
        now = datetime.datetime.now()
        year = now.year
        month = now.month
        return redirect(url_for('schedule', year=year, month=month))
@app.route("/")
@login_required
def index():
        admin=db.execute("select ad_username from Admin where ad_id=?;",session["user_id"])
        current_day=datetime.datetime.now().strftime('%Y-%m-%d')
        active_member=db.execute("select cl_Fname,cl_Lname,cl_num,sp_name from member where starts_at<=? and ends_at >?",current_day,current_day)
        now = datetime.datetime.now()
        year = now.year
        month = now.month
        cal=generate_calendar(year,month)
        #getting the events 
        events=db.execute("select * from schedule where start_date<=? and end_date >=?;",current_day,current_day)
        current_events={}
        #iterating for each event in this month
        for activity in events:
            activity["sp_name"]=db.execute("select sp_name from sport where sp_id=?",activity["sp_id"])
            if activity["repeat_pattern"]!="irregular":
                #setting the condtions to display the events in the correct dates
       
                    #making the list of the days that event appear
                    days=[day for day in activity["days"].split(",") if day]
                    #iterating for every week
                    for week in cal:
                        #iterating for every day
                        for day in range(7):
                            #checking for days that match the repeat pattern
                            if str(day) in days and week[day]!=0 :
                                #checking if we are before the start day if it's the same year and month (that's why we put not)
                                start_day_condition=(int(activity["start_date"][0:4])==year and int(activity["start_date"][5:7])==month  and week[day]<int(activity["start_date"][8:]))
                                #checking that we are after the end day if it's the same year and month (that's why we put not)
                                end_day_condition=(int(activity["end_date"][0:4])==year and int(activity["end_date"][5:7])==month  and week[day]>int(activity["start_date"][8:]))
                                if not (start_day_condition) and not (end_day_condition ):
                                    #putting the event date and info in the dictionarry
                                    formatted_date = '{:04d}-{:02d}-{:02d}'.format(year, month,week[day])
                                    current_events[str(activity["sc_id"])+formatted_date]=[formatted_date,activity["sp_name"][0]["sp_name"],activity["starts_at"],activity["ends_at"]]
            else:
                #for the irregular events
                current_events[str(activity["sc_id"])+activity["start_date"]]=[activity["start_date"],activity["sp_name"][0]["sp_name"],activity["starts_at"],activity["ends_at"]]
        return render_template("index.html",active_member=active_member,current_events=current_events,current_day=current_day,admin=admin)


@app.route('/sport', methods=["GET", "POST"])
@login_required
def sport():
    def add():
        #checking if the name already exist 
        names=db.execute("select sp_id from sport where sp_name=?",request.form.get("sp_name"))
        if len(names)>0:
            return "sport already exist"
        db.execute("insert into sport (sp_name,max_capacity) values (?,?)",request.form.get("sp_name"),request.form.get("max_capacity"))
        

    def add_c():
        sp_id=db.execute("select sp_id from sport where sp_name=?",request.form.get("sp_name"))
        db.execute("insert into coaches (co_id,sp_id,starts_at,ends_at) values(?,?,?,?)",
                       request.form.get("co_id"),
                       sp_id[0]["sp_id"],
                       request.form.get("starts_at"),
                       request.form.get("ends_at"))

    
    def edit():
        dict_sport={"sp_name":request.form.get("sp_name"),"max_capacity":request.form.get("max_capacity")}
        for key,value in dict_sport.items():
            db.execute("update sport set ?=? where sp_id=?",key,value,request.form.get("sp_id"))
         
    def delete():
        db.execute("delete from sport where sp_id=?",request.form.get("sp_id"))
    
    
    coach=db.execute("select co_id,co_Fname,co_Lname from coach;")
    sports_with_no_coach=db.execute("select sp_id,sp_name,max_capacity from sport where sp_id not in(select sp_id from coaches);")
    coaching_view=db.execute("select sp_id,sp_name,max_capacity,GROUP_CONCAT(co_Fname || ' ' || co_Lname,', ') as coach_list  from coaching_view group by sp_name;")
    if request.method=="GET":
        
        return render_template("sport.html",coach=coach,coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)
    else :
        if request.form.get("form_id")=="formsp-add":
            result=add()
            if result is None:
                if request.form.get("co_id"):
                    add_c()
                    return render_template("sport.html",coach=coach,success="sport and his coach added successfuly",coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)

                else:
                    return render_template("sport.html",coach=coach,success="sport added successfuly",coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)
            else:
                return render_template("sport.html",coach=coach,error=result,coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)

            
           
        elif "formsp-edit" in request.form.get("form_id"):
            if request.form.get("form_id")=="formsp-edit-withoutcoach":

                edit()
                if request.form.get("co_id"):
                    add_c()  
                    return render_template("sport.html",coach=coach,success="sport edited successfuly, Coach added successfuly",coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)

                else:
                    return render_template("sport.html",coach=coach,success="sport edited successfuly",coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)
            else:
                edit()
                return render_template("sport.html",coach=coach,success="sport edited successfuly",coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)
                  
        else:
            delete()
            return render_template("sport.html",coach=coach,success="sport deleted successfuly",coaching_view=coaching_view,sports_with_no_coach=sports_with_no_coach)


@app.route('/coach', methods=["GET", "POST"])
@login_required
def coach():
    coaching_view=db.execute("select * from coaching_view;")
    
    sport=db.execute("select sp_id,sp_name from sport;")

    def add():
        if (not request.form.get("co_num").isnumeric()) or len(request.form.get("co_num"))<8:
                return "phone number invalid"
            #checking the phone number already exist
        number=db.execute("select co_Fname from coach where co_num=?;",request.form.get("co_num"))
        if len(number)>0:
            return "phone number already exist"
        if request.form.get("salary"):

            if (not request.form.get("salary").isnumeric()):
                return "salary invalid"
        db.execute("insert into coach (co_Fname,co_Lname,co_num,salary,paid_at) values (?,?,?,?,?)",
                   request.form.get("co_Fname"),
                   request.form.get("co_Lname"),
                   request.form.get("co_num"),
                   request.form.get("salary"),
                   request.form.get("paid_at")
                    )
        co_id=db.execute("select co_id from coach where co_num=?",request.form.get("co_num"))
        db.execute("insert into coaches (co_id,sp_id,starts_at,ends_at) values(?,?,?,?)",
                   co_id[0]["co_id"],
                   request.form.get("sp_id"),
                   request.form.get("starts_at"),
                   request.form.get("ends_at"))
    def edit():
        if request.form.get("co_num"):
            if (not request.form.get("co_num").isnumeric()) or len(request.form.get("co_num"))<8:
                    return "phone number invalid"
            #checking the phone number already exist
            number=db.execute("select co_Fname from coach where co_num=?;",request.form.get("co_num"))
            if len(number)>0:
                return "phone number already exist"
        if request.form.get("salary"):

            if (not request.form.get("salary").isnumeric()):
                return "salary invalid"
        dict_coach={"co_Fname":request.form.get("co_Fname"),"co_Lname":request.form.get("co_Lname"),"co_num":request.form.get("co_num"),
                    "salary":request.form.get("salary"),"paid_at":request.form.get("paid_at")}
        dict_coaches={"sp_id":request.form.get("sp_id"),"starts_at":request.form.get("starts_at"),"ends_at":request.form.get("ends_at")}
        for key,value in dict_coach.items():
            if value:
                db.execute("update coach set ?=? where co_id=?;",key,value,request.form.get("co_id"))
        for key,value in dict_coaches.items():
            if value:
                db.execute("update coaches set ?=? where co_id=?;",key,value,request.form.get("co_id"))
    def delete():
        db.execute("delete from coach where co_id=?",request.form.get("co_id"))
        
   
    if request.method=="GET":
        return render_template("coaches.html",sport=sport,coaching_view=coaching_view)
    else:
        if request.form.get("form_id")=="formco-add":
            result=add()
            if result is None:
                return render_template("coaches.html",success="coach added successfuly",sport=sport,coaching_view=coaching_view)
            else:
                return render_template("coaches.html",error=result,sport=sport,coaching_view=coaching_view)
        elif request.form.get("form_id")=="formco-edit":
            result=edit()
            if result is None:
                return render_template("coaches.html",success="coach edited successfuly",sport=sport,coaching_view=coaching_view)
            else:
                return render_template("coaches.html",error=result,sport=sport,coaching_view=coaching_view)
        else :
            delete()
            return render_template("coaches.html",success="coach deleted successfuly",sport=sport,coaching_view=coaching_view)
          
       