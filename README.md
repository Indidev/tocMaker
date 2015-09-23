# tocMaker [![Coverity Scan Build Status](https://scan.coverity.com/projects/6410/badge.svg)](https://scan.coverity.com/projects/indidev-tocmaker)
creates a table of contents for your github readme file.

The following table of contents is created by this tool:

<!-- [toc] -->
## Table of Contents

   * [Installation](#installation)
   * [Usage Info](#usage-info)
   * [Example Headline](#example-headline)
     * [subitem1](#subitem1)
     * [subitem2](#subitem2)
   * [Example Headline 2](#example-headline-2)
     * [subitem1](#subitem1-1)
     * [subitem2](#subitem2-1)

<!-- [toc] -->
## Installation
just use ```make```

## Usage Info
To create a table of content use ```tocMaker <pathToYourReadme>```

Example: ```tocMaker README.md```

Your table of contents will be created before the first headline that starts with **##** and overrides an existing table of contents .

## Example Headline
here could be some text
### subitem1
### subitem2

## Example Headline 2
here could be some text
### subitem1
### subitem2
