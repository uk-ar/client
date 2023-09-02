usage:
- client ip read
- client ip write content
- client ip insert content position

example:
- client 127.0.0.1 read              'read data from localhost'
- client 127.0.0.1 write content     'write "content" to the file in localhost'
- client 127.0.0.1 insert content 3  'insert "content" at offset 3 from the beginning

limitations:
- only support text data
  - null charactor(\0) may cause unexpected error
- limit to small size file(up to 1024 bytes)
