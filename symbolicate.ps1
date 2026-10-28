$ErrorActionPreference = 'Stop'

# Make dbghelp.dll discoverable for P/Invoke
$env:PATH = "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64;" + $env:PATH

$cs = @'
using System;
using System.Runtime.InteropServices;

public static class Sym
{
    [DllImport("kernel32.dll")]
    public static extern IntPtr GetCurrentProcess();

    [DllImport("dbghelp.dll", SetLastError = true, CharSet = CharSet.Ansi)]
    public static extern bool SymSetOptions(uint options);

    [DllImport("dbghelp.dll", SetLastError = true, CharSet = CharSet.Ansi)]
    public static extern bool SymInitialize(IntPtr hProcess, string searchPath, bool invadeProcess);

    [DllImport("dbghelp.dll", SetLastError = true, CharSet = CharSet.Ansi)]
    public static extern ulong SymLoadModuleEx(IntPtr hProcess, IntPtr hFile, string imageName, string moduleName, long baseOfDll, int dllSize, IntPtr data, uint flags);

    [DllImport("dbghelp.dll", SetLastError = true, CharSet = CharSet.Ansi)]
    public static extern bool SymFromAddr(IntPtr hProcess, ulong address, out ulong displacement, IntPtr symbol);

    [DllImport("dbghelp.dll", SetLastError = true, CharSet = CharSet.Ansi)]
    public static extern bool SymGetLineFromAddr64(IntPtr hProcess, ulong address, out uint displacement, out IMAGEHLP_LINE64 line);

    [DllImport("dbghelp.dll", SetLastError = true)]
    public static extern bool SymCleanup(IntPtr hProcess);

    [StructLayout(LayoutKind.Sequential)]
    public struct IMAGEHLP_LINE64
    {
        public uint SizeOfStruct;
        public IntPtr Key;
        public uint LineNumber;
        public IntPtr FileName;
        public ulong Address;
    }
}
'@

Add-Type -TypeDefinition $cs

$exe     = "F:\ParallelsGames\LittleLightsBuilds\LL_0.9.0.9i\LittleLights\Binaries\Win64\LittleLights-Win64-Shipping.exe"
$symPath = "F:\ParallelsGames\LittleLightsBuilds\LL_0.9.0.9i\LittleLights\Binaries\Win64"
$base    = [long]0x7ff6dfe90000

$proc = [Sym]::GetCurrentProcess()
[void][Sym]::SymSetOptions(0x2 -bor 0x10)   # SYMOPT_UNDNAME | SYMOPT_LOAD_LINES
$init = [Sym]::SymInitialize($proc, $symPath, $false)
Write-Host ("SymInitialize: " + $init)

$mod = [Sym]::SymLoadModuleEx($proc, [IntPtr]::Zero, $exe, "LittleLights-Win64-Shipping", $base, 0, [IntPtr]::Zero, 0)
Write-Host ("SymLoadModuleEx base: 0x{0:X}" -f $mod)

$offsets = @(0x12ac530,0x3f68595,0x3f73b78,0x11a1339,0x12ab845,0x12ae408,0x12ae311,0x11a1339,0x12ae1bf,0x3222bf2,0x379d63a,0x3792385,0x322dafb,0x322db66,0x3212d96,0x3c6d047,0xf41f1e,0xf48375,0x3c83330,0x3c88485,0x379cc40,0x361d0dc,0x3da2344,0x3da990c,0x3da998a,0x3daa7b0,0x3db2a94,0x5cb1462)

$buf = [Runtime.InteropServices.Marshal]::AllocHGlobal(4096)
foreach ($o in $offsets) {
    [Runtime.InteropServices.Marshal]::WriteInt32($buf, 0, 88)         # SizeOfStruct
    [Runtime.InteropServices.Marshal]::WriteInt32($buf, 80, 4096 - 84) # MaxNameLen
    $addr = [UInt64]($base + $o)
    $disp = [UInt64]0
    if ([Sym]::SymFromAddr($proc, $addr, [ref]$disp, $buf)) {
        $name = [Runtime.InteropServices.Marshal]::PtrToStringAnsi([IntPtr]($buf.ToInt64() + 84))
        $ld = [UInt32]0
        $line = New-Object Sym+IMAGEHLP_LINE64
        $line.SizeOfStruct = [UInt32][Runtime.InteropServices.Marshal]::SizeOf([type][Sym+IMAGEHLP_LINE64])
        $file = ""
        if ([Sym]::SymGetLineFromAddr64($proc, $addr, [ref]$ld, [ref]$line)) {
            if ($line.FileName -ne [IntPtr]::Zero) { $file = [Runtime.InteropServices.Marshal]::PtrToStringAnsi($line.FileName) }
        }
        Write-Host ("+{0:x}  {1} +0x{2:x}   {3}:{4}" -f $o, $name, $disp, $file, $line.LineNumber)
    } else {
        $err = [Runtime.InteropServices.Marshal]::GetLastWin32Error()
        Write-Host ("+{0:x}  <no symbol> err={1}" -f $o, $err)
    }
}
[Runtime.InteropServices.Marshal]::FreeHGlobal($buf)
[void][Sym]::SymCleanup($proc)
