# tocMaker
creates a table of content for your github readme file

The following table of content is created by this tool:

## Table of Content

   * [Installation](#installation)
   * [Usage Info](#usage-info)
   * [Example Headline](#example-headline)
     * [subitem1](#subitem1)
     * [subitem2](#subitem2)
   * [Example Headline 2](#example-headline-2)
     * [subitem1](#subitem1_1)
     * [subitem2](#subitem2_1)

## Installation
just use ```make```

## Usage Info
To create a table of content use ```tocMaker <pathToYourReadme>```

Example: ```tocMaker README.md```
Your table of content will be created before the first headline which starts with ##.

**Attention:** It is recommended to delete the old table of content before using the tool, automated deletion is not yet implemented.

## Example Headline
here could be some text
### subitem1
### subitem2

## Example Headline 2
here could be some text
### subitem1
### subitem2
