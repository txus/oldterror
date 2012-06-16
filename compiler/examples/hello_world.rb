a = 123
a.message = [{ msg: "Hello world!"}, { msg: "Goodbye world!"}]
b = false
if b
  puts a.message[1]['msg']
else
  puts a.message[0]['msg']
end