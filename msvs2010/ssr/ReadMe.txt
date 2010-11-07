
Simple Search and Replace Command Line Tool
-------------------------------------------

Sometimes you just need a very simple tool to search 
and replace text in text files. This is such a tool. 


Usage: ssr <occurrence> <search_text> <replace_text> 
           <input_file> [<output_file>]

Where:
  <occurrence> = An integer specifying which occurrence 
		 to replace, use 0 for all.

Notes:
  Use '' to represent " in search and replace text.
  Use /SSR_QUOTE/ to represent ' in search and replace text.
  Use /SSR_NL/ to represent a new line in search and replace text.
  Use /SSR_TAB/ to represent a tab in search and replace text.


This tool is built to be used in batch files so it uses a simple
method to replace quotation marks (") for the text arguments.

Normally one wold use a tool like sed for the job. While such a 
power tool can be used, it often requires escaping many characters
and can be difficult to work with if you need to replace long
lines of text.


Author: Ross MacGregor
Date: July, 2005
License: Freeware, Unrestricted Use
E-mail: rmacgregor@ngrain.com


History
-------

Version 1.2: November 16, 2005
  - Improved error handling
