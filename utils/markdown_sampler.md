
# DBJ Visual Code Markdown Sampler

**TOC <!-- omit in toc -->**

- [DBJ Visual Code Markdown Sampler](#dbj-visual-code-markdown-sampler)
- [Section 1](#section-1)
- [Section 2](#section-2)
  - [Sub section 2.1](#sub-section-21)
- [Source Code 3](#source-code-3)
  - [Single Line 3.1](#single-line-31)
  - [Multi Line 3.2](#multi-line-32)
- [HTML Table + Source Code](#html-table--source-code)

# Section 1

The indentation rules (tab or spaces) of TOC will be the same of your current file (find it in the right bottom corner)

## Sub section 1.1<!-- omit in toc -->

# Section 2

To make TOC compatible with GitHub, you need to set option githubCompatibility to true

## Sub section 2.1

Use `<!-- omit in toc -->` to ignore specific heading in TOC

# Source Code 3

## Single Line 3.1

| Lorem                     | Ipsum                                  |
| :------------------------ | :------------------------------------- |
| `Source Code Single Line` | `Another source here also single line` |

## Multi Line 3.2

| Lorem                          | Ipsum                                      |
| :----------------------------- | :----------------------------------------- |
| `Source Code`<br> `Multi Line` | `Another source here`<br>`also multi line` |

# HTML Table + Source Code

<table style="table-layout: fixed!important;width:100%!important;white-space:nowrap!important;margin:0px!important;padding:0px!important" >
<tr >
<th style="width:50%!important;overflow:hidden!important;white-space:nowrap!important;text-overflow!important: ellipsis;" >Lorem</th>
<th style="width:50%!important;overflow:hidden!important;white-space:nowrap!important;text-overflow: ellipsis!important;" >Ipsum</th></tr>
<tr  >
<td  valign="top">
    
```cpp
// code fences must be 
// A: with one line above the starting one
// B: starting at column 0
// C: table inline HTML is best be positioned
// at column 0 too!
// this is source code
std::string upper( std::string_view sv_)
{
  return to_upper(sv_);
}
```
</td><td valign="top">

```cpp
// this **is** working
std::string upper( std::string_view sv_)
{
  return some_magic(sv_);
}
```
</td></tr>
<tr>
<td>Additional comments here.</td>
<td>Some more text here.</td>
</tr>
</table>
