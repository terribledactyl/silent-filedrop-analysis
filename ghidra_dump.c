int __cdecl main(int _Argc,char **_Argv,char **_Env)

{
  DWORD nNumberOfBytesToWrite;
  DWORD local_13c;
  CHAR local_138 [264];
  char *local_30;
  HANDLE local_28;
  char *local_20;
  char *local_18;
  HWND local_10;
  
  __main();
  local_10 = GetConsoleWindow();
  // Retrieves the window handle used by the console associated with the calling process.
  ShowWindow(local_10,0);
  // Sets the specified window's show state. This is hiding the window.
  local_18 = "C:\\Users\\Public\\";
  // Hardcoded path, likely output path.
  local_20 = "out.txt";
  // Hardcoded output file.
  lstrcpyA(local_138,"C:\\Users\\Public\\");
  // Copies string (unknown variable) into buffer
  lstrcatA(local_138,local_20);
  // Appends one string to another
  local_28 = CreateFileA(local_138,0x40000000,0,(LPSECURITY_ATTRIBUTES)0x0,2,2,(HANDLE)0x0);
  // **FILE_FLAG_OVERLAPPED** The file or device is being opened or created for asynchronous I/O.
  if (local_28 != (HANDLE)0xffffffffffffffff) {
  // 64 bit unsigned integer
    local_30 = "Reverse me.";
    nNumberOfBytesToWrite = lstrlenA("Reverse me.");
    WriteFile(local_28,local_30,nNumberOfBytesToWrite,&local_13c,(LPOVERLAPPED)0x0);
    // Writes data to the specified file or input/output (I/O) device.
    CloseHandle(local_28);
  }
  return 0;
}

/* initial thoughts
This is getting the local console window and hiding it, then quietly creating a text file at C:\Users\Public\out.txt with 'reverse me' text - need to do dynamic analyis to determine some of the unknown variables

Dynamic analysis confirms this creates a text file at C:\Users\Public\out.txt with the contents "Reverse me." in the file. Running this will briefly flash the console but close it almost immediately, followed by the creation of the file. */
