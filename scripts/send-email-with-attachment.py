# Reference: https://stackoverflow.com/questions/3362600/how-to-send-email-attachments

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders

mail_content = '''
<put email body here>
'''

sender_address = '<sender address>'
sender_pass = '<sender password>'
receiver_address = '<receiver address>'

message = MIMEMultipart()
message['From'] = '<username to display>'
message['To'] = receiver_address
message['Subject'] = '<email subject>'

message.attach(MIMEText(mail_content, 'plain'))
attachment = '<full path of mail attachment, use double slashes to escape slashes in path>'
attach_file = open(attachment, 'rb')
payload = MIMEBase('application', 'octate-stream')
payload.set_payload((attach_file).read())
encoders.encode_base64(payload)

payload.add_header('Content-Disposition', 'attachment; filename="<hardcode filename with extension>"')
message.attach(payload)

session = smtplib.SMTP('<smtp server or open-relay>', <port>)
session.starttls()
session.login(sender_address, sender_pass)
text = message.as_string()
session.sendmail(sender_address, receiver_address, text)
session.quit()
