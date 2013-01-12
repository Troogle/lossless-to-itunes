Public Class Form1
    ' VB -->首次打开提示设置
    ' VB+lib7z-->拖入rar/zip/7z,解压压缩包内jpg
    ' VB+lib7z-->输出cue，选择其中一项（要有预览（含音乐文件提示）)
    ' VB+libcue(VC)-->对cue文件进行处理（转码+改名）
    ' VB+lib7z-->输出音乐文件
    ' VB+libtrans(VB)-->执行转换
    ' libtrans-->libflac+libtta+libtak+libape
    ' VB+libmount-->mount
    ' VB+libmount-->unmount
    ' VB+lib7z-->重新压缩


    Public Declare Function convertape Lib "libtrans.dll" Alias "?convertape@@YGHPBD0@Z" (ByRef a As String, ByRef b As String) As Long
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        convertape("1.ape", "1.wav")
    End Sub
End Class
