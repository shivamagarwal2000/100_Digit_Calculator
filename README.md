# 100_Digit_Calculator

Idea - I thought as C's compiler can only handle calculation in the integer range or a long int for extension. But more often
than not we require an extended precision calculator. So I developed a simple calculator that can handle upto 100 digits(can be extended) and perform simple +, -, *, / and ^ tasks on these large digit numbers.

The implementation uses array as an underlying data structure and I have kept the length of the array as 100 just for this project which can be increased/decreased for future puposes. This implementation is both fast and space efficient.

The logic for the project revolves around using actual priciple of adding/subtracting etc single digits one at a time. This logic is similar to the one we do by our hand. I have made sure the algorithm is optimized so that it can actually compute such digits efficiently.
