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

waiting_times = [0]
for i in range(1, len(times)):
    waiting_times.append(sum(times[:i]))

average_waiting_time = sum(waiting_times) / len(waiting_times)

average_execution_time = sum(times) / len(times)

print(f"Среднее время ожидания: {average_waiting_time:.2f}")
print(f"Среднее время выполнения: {average_execution_time:.2f}")

times = [
    count_vowels(splitted_fio[0]),
    count_consonants(splitted_fio[0]),
    count_vowels(splitted_fio[1]),
    count_consonants(splitted_fio[1]),
    count_vowels(splitted_fio[2]),
    count_consonants(splitted_fio[2]),
]

process_data = pd.DataFrame({
    'Номер процесса': range(1, 7),
    'Вычисленное время': times
})

print(process_data.to_string(index=False))

waiting_times = [0]
for i in range(1, len(times)):
    waiting_times.append(waiting_times[-1] + times[i-1]) 

average_waiting_time = sum(waiting_times) / len(waiting_times)

waiting_times_table = waiting_times + [average_waiting_time]

average_execution_time = sum(times) / len(times)

times_table = times + [average_execution_time]

process_numbers = list(range(1, len(times) + 1)) + ["Ср. вр."]

df = pd.DataFrame({
    '№ процесса': process_numbers,
    'ожидание': waiting_times_table,
    'выполнение': times_table
})

df['ожидание'] = df['ожидание'].apply(lambda x: f"{x:.1f}")
df['выполнение'] = df['выполнение'].apply(lambda x: f"{x:.1f}")

df = df.set_index('№ процесса').T

print(df)
