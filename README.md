# Valgrind_log_tool

Continuous integration with [Travis-Ci](https://app.travis-ci.com/github/quicky2000/valgrind_log_tool) : ![Build Status](https://app.travis-ci.com/quicky2000/valgrind_log_tool.svg?branch=master)

This tool parse valgrind XML logs and generate an HTML page to make exploration easier
Typical way to generate valgrind memcheck XML log is the following

```valgrind --tool=memcheck --leak-check=full -v --xml=yes --xml-file=report.xml```
