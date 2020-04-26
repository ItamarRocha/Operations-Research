with open('country.txt', 'r') as file :
  filedata = file.read()
  
filedata = filedata.replace("\'","\"")



with open('country.txt', 'w') as file :
    file.write(filedata)