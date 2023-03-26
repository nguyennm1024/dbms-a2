import os
import shutil
import matplotlib.pyplot as plt

def read_words_from_file(file_name):
    with open(file_name, 'r') as f:
        content = f.read()
    return content

keywords = ["Oregon", "Competitive", "Services", "program", "LSTA", "federal", "allotment", "through", "Institute", "Museum",
"funds", "from", "State", "funded", "grant", "projects", "Library", "Technology", "Art", "and"]

metadata = open("metadata.txt", "r")

corpus = [500, 10000, 15000, 20000, 25000, 30000, 35000, 40000]

cardinality_words = [100, 200, 300, 400, 500]

cardinality_k = [2, 10, 20, 25, 40]

for i in corpus:
    file = open(str(i) + ".txt", "w")
    for j in range(i):
        file.write(metadata.readline())
    file.close()
    metadata.seek(0, 0)

metadata.close()



indexer_time = []
searcher_time = []

for i in corpus:
    # out_file_index = " >" + str(i) + "Index.txt"
    index_cmd = "./metadata_indexer " +  "metadata.txt " + "database " + str(i)
    r = os.popen(index_cmd)
    output =r.readline()
    if output.strip():
            indexer_time.append(float(output))

    r.close()
    # out_file_search = " >" + str(i) + "Search.txt"
    # os.system("./searcher database 10 " + keywords[0] + " " + keywords[1])
    search_cmd = "./metadata_search database 10 " + keywords[0] + " " + keywords[1]
    r = os.popen(search_cmd)
    output =r.readline()
    searcher_time.append(float(output))
    r.close()
    print("Size " + str(i) + " test finished")
    shutil.rmtree("database")


print(indexer_time)
print(searcher_time)

plt.plot(corpus, indexer_time)
plt.ylabel("indexing time")
plt.xlabel("corpus size")
plt.xlim(100, 45000)
plt.ylim(0, 30)
plt.title("Indexing time versus data size")
plt.savefig('assets/1.jpg')
plt.close()

plt.plot(corpus, searcher_time)
plt.ylabel("search time(s)")
plt.xlabel("corpus size")
plt.xlim(100, 45000)
plt.ylim(0, 0.001)
plt.title("Search time versus data size")
plt.savefig('assets/2.jpg')
plt.close()


search_test_AND = []
search_test_OR = []

corpus_size = [2, 5, 10, 15, 20]

os.system("./metadata_indexer " + "metadata.txt " + "database " + "40000")

for i in corpus_size:
    search_cmd = "./metadata_search database 10 "
    for j in range(i):
        search_cmd += keywords[j] + " "
    r = os.popen(search_cmd)
    search_test_OR.append(float(r.readline()))
    r.close()

plt.plot(corpus_size, search_test_OR)
plt.ylabel("search time(s)")
plt.xlabel("corpus size")
plt.xlim(2, 20)
plt.ylim(0, 0.001)
plt.title("Search time versus number of OR-keywords in the query")
plt.savefig('assets/4.jpg')
plt.close()

for i in corpus_size:
    search_cmd = "./metadata_search database 10 "
    for j in range(i):
        search_cmd += "+" + keywords[j] + " "
    r = os.popen(search_cmd)
    search_test_AND.append(float(r.readline()))
    r.close()

plt.plot(corpus_size, search_test_AND)
plt.ylabel("search time(s)")
plt.xlabel("corpus size")
plt.xlim(2, 20)
plt.ylim(0, 0.001)
plt.title("Search time versus number of AND-keywords in the query")
plt.savefig('assets/3.jpg')
plt.close()

print(search_test_AND)
print(search_test_OR)

search_test_card = []

os.system("python word_generator.py")

for i in cardinality_words:
    file_name = f'{i}_words.txt'
    words_string = read_words_from_file(file_name)
    search_cmd = "./metadata_search database 10 " + str(i)
    r = os.popen(search_cmd)
    search_test_card.append(float(r.readline()))
    r.close()
    
print(search_test_card)

plt.plot(cardinality_words, search_test_card)
plt.ylabel("search time(s)")
plt.xlabel("cardinality_words")
plt.xlim(0, 500)
plt.ylim(0, 0.001)
plt.title("Search time versus number of words")
plt.savefig('assets/5.jpg')
plt.close()
    
search_test_k = []
    
file_name = f'100_words.txt'
words_string = read_words_from_file(file_name)
for k in cardinality_k:
    search_cmd = "./metadata_search database " + str(k) + " " + words_string
    r = os.popen(search_cmd)
    search_test_k.append(float(r.readline()))
    r.close()

print(search_test_k)


plt.plot(cardinality_k, search_test_k)
plt.ylabel("search time(s)")
plt.xlabel("cardinality_k")
plt.xlim(0, 40)
plt.ylim(0, 0.01)
plt.title("Search time versus k")
plt.savefig('assets/6.jpg')
plt.close()

shutil.rmtree("database")
