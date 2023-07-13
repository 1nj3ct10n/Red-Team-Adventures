Sometimes you just need to get to things remotely. Those things might be behind an unbudging firewall with no way to forward proper ports for remote access, or a pentest dropbox connected to public WiFi network or mobile data having a dynamic public IP address.
<br>
How to SSH into the dropbox to carry out attacks on the network?
<br>
Solution: Creating a reverse SSH tunnel. Instead of the command and control server connecting to the Raspberry Pi, the Raspberry Pi initiates the connection to the command and control server. This is the same technique used by many backdoor programs.

Original credits:
<br>
https://charlesreid1.com/wiki/RaspberryPi/Reverse_SSH
<br>
https://hobo.house/2016/06/20/fun-and-profit-with-reverse-ssh-tunnels-and-autossh/
<br>
http://www.vdomck.org/2005/11/reversing-ssh-connection.html
