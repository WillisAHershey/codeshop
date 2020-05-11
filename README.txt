The goal of this project is to create a text editor coding environment that can do the following things:

Open and edit multiple user-level files, and create new user-level files
Access and read read-only operating system files without administrator permissions
Recognize keywords defined by some given programming language
Create new keywords based on definitions in files
Display the files being read and edited in a logical and easy-to-understand way
Offer definitions of the user-defined keywords by scanning associated files
Be compilable and runable on Windows, Linux, and Apple devices.

When a user-level file is opened it should:

Be opened in such a way that it can be safely edited without harming the original until the user decides to write changes into the file
Be opened in a portable (non-implementation specific) way
	fopen fprintf and fclose will provide the portable file editing
Three distinct guis will be written in the userInterface.c file
	The Microsoft Windows gui will be written using the Windows interface
	The Linux gui will be written using the X11 interface
	and the MacOs gui will be written using the Quartz interface
	


