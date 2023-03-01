table_from_c = get_c_table()
print(table_from_c["person"]["name"])
print(table_from_c["person"]["age"])
print(table_from_c[1])
print(table_from_c[2])

c_table_reader({person={name = "carlos",age = 23}})