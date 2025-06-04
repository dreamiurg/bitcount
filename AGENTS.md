Repository Agent Guidelines
==========================

The purpose of this AGENTS.md is to outline how automated agents should
interact with this repository. Follow these instructions whenever you
create a pull request or modify files.

Coding style
------------
* Use **two spaces** for indentation. Do not use tabs.
* Keep line length under **100 characters** where possible.
* Prefer modern C++11 features and avoid platform specific extensions.

Testing
-------
* Always build and run the test suite before committing changes when any
  source files (`*.cpp`, `*.h`) or the `Makefile` are modified.
* Tests are executed with:

  ```sh
  make test
  ```

Commit messages
---------------
* Start with a short one line summary (under 50 characters) written in
  the imperative mood.
* Optionally include a blank line followed by more detailed explanation.

Pull request description
-----------------------
* Summarize what was changed and why.
* Mention the outcome of the test suite.

