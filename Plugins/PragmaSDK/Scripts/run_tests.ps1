
Write-Output ""

$uprojectfile = $args[0]
if (!(Get-Command "UE4Editor.exe")) {
    Write-Output "Could not find UE4Editor.exe on in the environment PATH."
    Write-Output "Usage: .\run_tests.ps1 C:\path\to\uproject"
    Write-Output ""
    exit 1
}
if (($null -eq $uprojectfile) -or !(Test-Path $uprojectfile)) {
    Write-Output "Could not find uproject file at absolute path: $uprojectfile"
    Write-Output "Usage: .\run_tests.ps1 C:\path\to\uproject"
    Write-Output ""
    exit 1
}

function CleanupLog($logs) {
    $logs = $logs -creplace ".* Display: Test Completed. (.*)", '$1'
    $logs = $logs -creplace ".* Error: Test Completed. (.*)", '$1'
    # Remove everything before " - PragmaSDK." as it's redundant.
    $logs = $logs -creplace ".* - (PragmaSDK.*)", '$1'
    # Remove everything before "Expected '" as it's redundant, and indent.
    $logs = $logs -creplace ".* (Expected '.*)", '    $1'
    $logs = $logs -creplace ".* (Error: .*)", '    $1'
    # Remove the final ')' in each line, if any.
    $logs = $logs -creplace '(.*)\)[\r\n|\n]', '$1'
    $logs.TrimEnd(')')
}

Write-Output "Starting Unreal Editor to execute tests..."

# Run unreal editor with automation test arguments and wait for it to finish.
$testprefix = $args[1]
Write-Output "$testprefix"
if ($testprefix -eq $null) {
	$testprefix = "PragmaSDK"
}
$arguments = $uprojectfile + ' Empty -ExecCmds="Automation RunTests ' + $testprefix + '" -testexit="Automation Test Queue Empty" -PragmaTest -unattended -nopause -nosplash -nullrhi -nosound -log -log="PragmaSDK_run_tests.txt" -LogCmds="LogPragma Verbose"'
Start-Process UE4Editor.exe $arguments -Wait

Write-Output ""

# Print automation log lines.
$uprojectdir = Split-Path -Path $uprojectfile
$logfilepath = Join-Path $uprojectdir "Saved" "Logs" "PragmaSDK_run_tests.txt"
$logfile = Get-Content $logfilepath

$successtests = ($logfile | Select-String -Pattern "LogAutomationController: Display: Test Completed. Result={Passed}")
$successtestcount = $successtests.Length
Write-Output "Successful tests ($successtestcount):"
Write-Output ""
CleanupLog($successtests)
Write-Output ""

$failedtests = ($logfile | Select-String -Pattern "LogAutomationController: Error: Test Completed. Result={Failed}")
$failedtestcount = $failedtests.Length
Write-Output "Failed tests ($failedtestcount):"
Write-Output ""
# Log failures but also log their failed expectations.
$failurelogs = $logfile | Select-String -Pattern "LogAutomationController: Error: Test Completed. Result={Failed}|LogAutomationController: Error: "
CleanupLog($failurelogs)

$criticalerror = ($logfile | Select-String -Pattern "=== Critical error: ===")
if ($criticalerror.Length -gt 0) {
    Write-Output "CRASH ENCOUNTERED --- SEE LOGFILE: $logfilepath"
}

Write-Output ""
Write-Output "Done!"
Write-Output "See full log at: $logfilepath"
