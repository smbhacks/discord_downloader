# Discord File Downloader

## Overview

Discord Files Downloader is a command-line tool designed to simplify the process of downloading files based on the file extensions you want from Discord archive text files. It specifically targets links from cdn.discordapp.com and media.discordapp.net found within these text files. 

## Usage

```
discord_downloader [folder] [extensions]
```
- [folder]: A folder that contains text files with archived Discord channels or direct messages.
- [extensions]: Files with these extensions will be downloaded. Separate extensions with spaces. Do not include periods in the extensions.

## Example usage

```
discord_downloader my_discord_server jpg png mp4
```

In this example, the tool will search for text files in the folder named `my_discord_server` and download files with the extensions `jpg`, `png`, and `mp4` from the links contained within those files.

## Disclaimer

This tool is intended for personal use and should be used responsibly and in accordance with Discord's terms of service. The developer is not responsible for any misuse or unintended consequences of using this tool.

## Contributing

If you encounter any issues or have suggestions for improvement, feel free to let me know.