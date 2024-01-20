name1 = "Апальков Егор Владимирович"  # Процесс 1
name2 = "Аристов Владислав Евгеньевич"  # Процесс 2
name3 = "Безбородов Евгений Сергеевич"

name1A1Z26 = [1,17,1,13,30,12,16,3, 6,4,16,18, 3,13,1,5,10,14,10,18,16,3,10,25]
name2A1Z26 = [1,18,10,19,20,16,3, 3,13,1,5,10,19,13,1,3, 6,3,4,6,15,30,6,3,10,25]
name3A1Z26 = [2,6,9,2,16,18,16,5,16,3, 6,3,4,6,15,10,11, 19,6,18,4,6,6,3,10,25]

def divide_sequence_by_num(sequence):
  result = []
  for num in sequence:
    mod_six = num % 9
    result.append(mod_six)
  return [num for num in result if num != 0]

for i in range(3):
  name1A1Z26 = divide_sequence_by_num(name1A1Z26)
  name2A1Z26 = divide_sequence_by_num(name2A1Z26)
  name3A1Z26 = divide_sequence_by_num(name3A1Z26)

print(name3A1Z26)