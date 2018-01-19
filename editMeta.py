import re

file = open("meta.csv")

i = 1
training = ""
test = ""

for line in file:
    if   (re.match("(.*)person02(.*)", line) or re.match("(.*)person03(.*)", line)):
        string = line
        test = test + string
    elif (re.match("(.*)person05(.*)", line) or re.match("(.*)person06(.*)", line)):
        string = line
        test = test + string
    elif (re.match("(.*)person07(.*)", line) or re.match("(.*)person08(.*)", line)):
        string = line
        test = test + string
    elif (re.match("(.*)person09(.*)", line) or re.match("(.*)person10(.*)", line)):
        string = line
        test = test + string
    elif re.match("(.*)person22(.*)", line):
        string = line
        test = test + string
    else:
        string = line
        training = training + string
        
    
print "test:\n"+test
print "\n training:\n"+training

# for line in file:
#     if re.match("(.*)-(.*)-(.*)-(.*)-(.*)", line):
#         string = line
#         l = l + string
#     else:
#         string = "------"+line
#         l = l + string
        
#     if (i%25)==0:
#         l = l + '\n'
#     i = i + 1
    
# print l


# i = 1
# l = ""
# for line in file:
#     if re.match("(.*)boxing(.*)", line):
#         string = "0,"+line
#         l = l + string
#         # print string
#     elif re.match("(.*)handclapping(.*)", line):
#         string = "1,"+line
#         l = l + string
#         # print string
#     elif re.match("(.*)handwaving(.*)", line):
#         string = "2,"+line
#         l = l + string
#         # print string
#     elif re.match("(.*)jogging(.*)", line):
#         string = "3,"+line
#         l = l + string
#         # print string
#     elif re.match("(.*)running(.*)", line):
#         string = "4,"+line
#         l = l + string
#         # print string
#     elif re.match("(.*)walking(.*)", line):
#         string = "5,"+line
#         l = l + string
#         # print string
#     # elif re.match("(.*)walking(.*)", line):
#     #     # print "walking" +" ",i
    
#     if (i%25)==0:
#         l = l + '\n'
#     i = i + 1
    
# print l
