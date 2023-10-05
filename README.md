# LR1parser
An LR(1) parser generator. Input an LR(1) grammar with space seperated tokens into grammarInput.txt, the first rule is the start rule. Numbers should not be tokens, reserved tokens are "$" and "%". Use "%" for epsilon. Input a stream of tokens into input.txt to syntactically analyze it. The program will output accept if there were no syntax errors, and report errors otherwise.
