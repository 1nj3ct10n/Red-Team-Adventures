Originally created by John Woodman (@JohnWoodman15), this VBA macro initially gives a reverse shell using only Windows API calls.
\
\
I modified it a bit according to my own needs. This macro does the following:
* Downloads and saves an additional c2 payload in an arbitrary location on Windows.
* Executes the payload to get a callback on our c2 (instead of a simple listener).
* Adds the payload in Userland Registry StartUp key for persistence.
* Creates a scheduled task to execute the payload after a desired period of time, for additional persistence.
* VirusTotal Scan Results: 4/57 (https://www.virustotal.com/gui/file/95b2bd3e00777d7ba490cb857055be2e69b847f95c9bd2b602f18c1f618c9600?nocache=1)
\
![image](https://user-images.githubusercontent.com/53261217/154839243-76814312-4d56-456e-84ec-7bcf916b13c7.png)
\
* AntiScan.me Scan Results: 1/26 (https://antiscan.me/images/result/Pi0zTQ4L7TGN.png)
\
![image](https://user-images.githubusercontent.com/53261217/154839621-e419bdab-8618-46fa-8c6e-062157613415.png)
\
\
Link to John's Socials:
\
Twitter: https://mobile.twitter.com/johnwoodman15
\
GitHub: https://github.com/JohnWoodman
\
John's Origianl Work: https://github.com/JohnWoodman/VBA-Macro-Reverse-Shell
