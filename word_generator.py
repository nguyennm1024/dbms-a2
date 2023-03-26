import random
import string
import nltk

nltk.download('words')
import random
from nltk.corpus import words

english_words = set(words.words())

def generate_random_word():
    return random.choice(list(english_words))

def add_plus_sign_randomly(word):
    if random.random() < 0.5:
        return '+' + word
    return word

def generate_words_file(word_count, file_name):
    if word_count > len(english_words):
        raise ValueError("The requested word count exceeds the number of available English words.")
    
    selected_words = random.sample(english_words, word_count)
    words_with_plus = [add_plus_sign_randomly(word) for word in selected_words]
    
    with open(file_name, 'w') as f:
        f.write(' '.join(words_with_plus))

word_counts = [100, 200, 300, 400, 500]

for count in word_counts:
    file_name = f'{count}_words.txt'
    generate_words_file(count, file_name)
    print(f"Generated file: {file_name}")
