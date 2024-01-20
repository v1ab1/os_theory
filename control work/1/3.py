import pandas as pd

def count_vowels(s):
    return sum(1 for c in s.lower() if c in 'аеёиоуыэюя')

def count_consonants(s):
    return sum(1 for c in s.lower() if c in 'бвгджзйклмнпрстфхцчшщ')

fio = "Аристов Владислав Евгеньевич"
splitted_fio = fio.split()

T1 = count_vowels(splitted_fio[0])
T2 = count_consonants(splitted_fio[0])
T3 = count_vowels(splitted_fio[1])
T4 = count_consonants(splitted_fio[1])
T5 = count_vowels(splitted_fio[2])
T6 = count_consonants(splitted_fio[2])

times = [T1, T2, T3, T4, T5, T6]

# Расчет момента поступления и приоритета для каждого процесса
arrival_times = [(9 - t) % 9 for t in times]
priorities = [(7 - t) % 7 for t in times]

# Расчет времени ожидания и времени выполнения
waiting_times = [0] * len(times)
for i in range(1, len(times)):
    # Время ожидания - это сумма времен выполнения всех предыдущих процессов
    waiting_times[i] = waiting_times[i - 1] + times[i - 1]

# Среднее время ожидания и среднее время выполнения
average_waiting_time = sum(waiting_times) / len(waiting_times)
average_execution_time = sum(times) / len(times)

# Создание DataFrame для процессов с временами ожидания и приоритетами
df = pd.DataFrame({
    'Номер процесса': range(1, 7),
    'Время выполнения': times,
    'Момент поступления': arrival_times,
    'Приоритет': priorities
})

# Расчет и вывод таблицы средних значений
df.loc['Ср. вр.'] = ['-', average_execution_time, '-', '-']
df['Время ожидания'] = waiting_times + [average_waiting_time]

# Применение форматирования к DataFrame
df[['Время выполнения', 'Время ожидания']] = df[['Время выполнения', 'Время ожидания']].applymap(lambda x: f"{x:.1f}")

print(df)
