if "%PYTHON_ARCH%"=="32" (
    echo f | xcopy /y "C:\Program Files (x86)\OpenSSL-Win32\lib\VC\static\libcrypto32MD.lib" "./openssl/lib/libcrypto.lib"
    echo d | xcopy /s /y "C:\Program Files (x86)\OpenSSL-Win32\include" "./openssl/include"
    echo "Files x86 copied successfully."
) else (
    echo f | xcopy /y "C:\Program Files\OpenSSL\lib\VC\static\libcrypto64MD.lib" "./openssl/lib/libcrypto.lib"
    echo d | xcopy /s /y "C:\Program Files\OpenSSL\include" "./openssl/include"
    echo "Files x64 copied successfully."
)
