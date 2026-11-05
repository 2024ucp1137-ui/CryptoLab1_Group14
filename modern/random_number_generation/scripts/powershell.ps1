$b = New-Object byte[] 1048576
$rng = [System.Security.Cryptography.RandomNumberGenerator]::Create()

try {
    $rng.GetBytes($b)
    [IO.File]::WriteAllBytes("$PWD\cng.bin", $b)
}
finally {
    $rng.Dispose()
}

(Get-Item .\cng.bin).Length
