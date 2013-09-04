Public Class Form1
    ' VB为底
    ' libcue(VC)-->对cue文件进行处理（转码+改名）
    ' libtrans(VC)-->执行转换
    ' libtrans-->all to wav &wav to ape
    Public Function AddExt(ByVal name As String, ByVal ext As String) As String
        Return (name.Remove(name.LastIndexOf(".")) & ext)
    End Function
    Private Sub encode(ByRef file As String)
        Dim outfile As String
        Dim ret As Integer
        outfile = AddExt(file, ".ape")
        ret = Shell("libtrans a " + """" + file + """" + " " + """" + outfile + """", AppWinStyle.NormalFocus, True)
    End Sub
    Private Sub dealfile(ByRef file As String)
        Dim ret As Integer
        Dim tempfile As String
        tempfile = AddExt(file, ".wav")
        Select Case System.IO.Path.GetExtension(file)
            Case ".tta"
                ret = Shell("libtrans t " + """" + file + """" + " " + """" + tempfile + """", AppWinStyle.NormalFocus, True)
                encode(tempfile)
                My.Computer.FileSystem.DeleteFile(tempfile, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
                My.Computer.FileSystem.DeleteFile(file, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
            Case ".tak"
                ret = Shell("tak -d " + """" + file + """" + " " + """" + tempfile + """", AppWinStyle.NormalFocus, True)
                encode(tempfile)
                My.Computer.FileSystem.DeleteFile(tempfile, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
                My.Computer.FileSystem.DeleteFile(file, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
            Case ".wav"
                encode(file)
                My.Computer.FileSystem.DeleteFile(file, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
            Case ".flac"
                ret = Shell("flac -d " + """" + file + """", AppWinStyle.NormalFocus, True)
                encode(tempfile)
                My.Computer.FileSystem.DeleteFile(tempfile, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
                My.Computer.FileSystem.DeleteFile(file, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin)
            Case ".cue"
                ret = Shell("libcue " + """" + file + """", AppWinStyle.NormalFocus, True)
        End Select
    End Sub
    Private Function checkit(ByRef name As String) As Boolean
        name = System.IO.Path.GetExtension(StrConv(name, VbStrConv.Lowercase))
        If name = ".cue" Or name = ".tak" Or name = ".tta" Or name = ".flac" Or name = ".ape" Or name = ".wav" Then
            Return True
        Else : Return False
        End If
    End Function
    Private Sub checkall()
        Dim i As Integer
        i = 0
        Do While i <= ListBox1.Items.Count - 1
            If Not checkit(ListBox1.Items.Item(i).ToString) Then
                ListBox1.Items.RemoveAt(i)
            Else
                i += 1
            End If
        Loop
    End Sub
    Private Sub TextBox1_DragDrop(sender As Object, e As DragEventArgs) Handles TextBox1.DragDrop
        ListBox1.Items.AddRange(DirectCast(e.Data.GetData(DataFormats.FileDrop), Array))
        checkall()
        If (ListBox1.Items.Count <> 0) Then Button1.Enabled = True
    End Sub

    Private Sub TextBox1_DragEnter(sender As Object, e As DragEventArgs) Handles TextBox1.DragEnter
        If e.Data.GetDataPresent(DataFormats.FileDrop) Then
            e.Effect = DragDropEffects.Link
        Else
            e.Effect = DragDropEffects.None
        End If
    End Sub
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Button1.Enabled = False
        Dim i As Integer
        i = 0
        Do While i <= ListBox1.Items.Count - 1
            If System.IO.Path.GetExtension(ListBox1.Items.Item(i).ToString) = ".cue" Then
                Dim temp As String = ListBox1.Items.Item(i).ToString
                ListBox1.Items.RemoveAt(i)
                ListBox1.Items.Insert(0, temp)
            Else
                i += 1
            End If
        Loop
        i = 0
        Do While i <= ListBox1.Items.Count - 1
            dealfile(ListBox1.Items.Item(i).ToString)
            ListBox1.Items.RemoveAt(i)
        Loop
    End Sub
End Class
