# Local2012Roblox
For playing Roblox 2012 versions without using a local webserver
It fixes broken corescripts, broken assets and broken play solo

# Usage
It pulls data from Content/localassets and Content/localstatic

localassets directory just contains the asset ids to replace instead of redirecting to assetdelivery

localstatic needs studio.ashx for corescript loading and visit.ashx for play solo

It uses version-e029025a3614426d (June 14, 2012) client however you can modify the addresses in source code to support other clients

You can download a ready made client that loads the DLL automatically and patches out signatures


# Building
You must use Visual Studio 2008 to build the DLL, as it uses no dependencies other than win32 all you need to do to compile is just to click build
