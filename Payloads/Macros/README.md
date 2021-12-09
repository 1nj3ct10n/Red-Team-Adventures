Originally created by John Woodman (@JohnWoodman15), this VBA macro initially gives a reverse shell using only Windows API calls.
\
\
I modified it a bit according to my own needs. This macro does the following:
* Downloads and saves an additional c2 payload in an arbitrary location on Windows.
* Executes the payload to get a callback on our c2 (instead of a simple listener).
* Adds the payload in Userland Registry StartUp key for persistence.
* Creates a scheduled task to execute the payload after a desired period of time, for additional persistence.
\
\
\
Link to John's Socials:
\
Twitter: https://mobile.twitter.com/johnwoodman15
\
GitHub: https://github.com/JohnWoodman
\
John's Origianl Work: https://github.com/JohnWoodman/VBA-Macro-Reverse-Shell
