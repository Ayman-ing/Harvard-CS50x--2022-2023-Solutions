
# Elite  Gym system  management
#### Video Demo:  https://youtu.be/cIlpgGeqMJo
#### Description:
So this is my cs50 final project , it's a local web application where I tried to focus on the back-end side of my application and no much at the front -end
so first I started by desinging my databbase using looping as same as I learned in my university trying as much as pssible to avoid redondance by using the normalization process.
Then I tried to set up phpliteadmin on my local server as I like from the last week (flask) wihch was a little bit challenging specially I'm not a friendly user for ubuntu (I installed wsl also on my windows).
So i used sqlite3 for this project and I started coding.
Honestly for the login and register Page I didn't change so much compared to the flask week as I think I don't find it challenging any more, I mean I want to focus on things newer and much more complicated,which I faced in member page and schedule page specially
I suffered so much to find a good and optimzed solution to add ,edit and delete members and memebershipswhich I thing the most page that took time from me.
then I faced an other difficulty in adding the schedule where I have to set up a calendar and show all the events in every month at their own days choosen,without forget the buttons for every cell of the schedule that made me lose my mind as they don't want to fit the cell.
for every page that needs add and edit and delete I made a specific function.
let's take the coaches page as exemple ,for adding coach I made a function called add for adding a new coach ,
so the adding form will be submiting for this page and that by  using a specific id for it ,an edit function for editing the coach which also has a specific id in the html template,and a delete function to delete a coach
One of them difficulties I faced is that I had to handle the get and post methods both and I have to avoid repeating the same code but sometimes it's not exactly the same so I can't make a specefic function for the code and call it twice,in several cases I succeeded in doing that but for few times I didn't.
also the variables that I sould provide for evere render_template which I thing there is a better solution then making so much variables I hope in the next projects I find it
It's necessary also to talk about jinja difficulties as it seems that she don't have axactly the same notation of python which make me strrugle in some situations looking for the documentation.
Also it seems that sqlite3 wasn't the best solution for me to handle my database ,it's right that it use a single file and so easy to handle it but honestly it's syntax is way different from mysql and due to my classes in the university of mysql , I struggled  a lot in finding the correct words for inserting updating and deleting.

in general I used the same method in the sport page and member page

As I mentioned before I tried to make the database normalized which is better for the database but harder to make queries and show data ,So I used the make views solution which commit multiple table data in one view that you can query from it and that made my mission much more easier
let's talk about the hardest part for me "the schedule"
at first I was suffering in making a calender than after research I found that there is a library called calendar from which you can import the calendar of any month  from any year you want ,so I used it.then I used datetime so much to import the actual year, month , day and even time by secondes, this helped me in the python code and evene in jinja .
but the challenging part was how to know for every day which events it has .
the answer is to generate form every month their days by the library calender ,then taking from the database the vents that still in progress(using the start_date and end_date) ,then checking for the days form the event if it  existe in the week index or not (the week index start from 0 to 6 and the had the day number correspand for every day name) if it exist we add it (I hope I made it clear)

for the templates I used ,I already mentioned member schedule, coach and sport
also we had the index page which didn't takes from me so much time as most of the problems I already solved it
so I showed the current events for today and the active members
for desinging the the website I used bootstrap as I don't have much knowledge about javascript ,so the modals or all from bootstrap ,some tables also some buttons ,but also I made some changes in the css using the styles.css file which includes the modifications I made for some tables,forms,titles etc..
also I used some functions from helpers to require loginning to avoid that someone without account can access for certain pages.
I used the flask session also to keep track of the current admin and not making him login every time he close server and rerun it.
In general ,I didn't made such a great design because as I used I don't know so much javascript and css , but I'm happy with the back-end side I made as I tried to make it omptimist organized and avoid as much as possible bugs

I'm Ayman and this was my cs50 final project
happy that I made it to the end :D

