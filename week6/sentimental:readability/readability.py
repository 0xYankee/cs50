# TODO
# L is the average number of letters per 100 words in the text
# S is the average number of sentences per 100 words in the text.

def main():
    text = get_text()
    letters = count_letters(text)                                    # Count letters
    words = text.count(" ") + 1                                      # Count words
    sentences = text.count(".") + text.count("!") + text.count("?")  # Count sentences

    # Calculate grade using the Coleman-Liau index
    index = round(0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8)
    if index < 1:
        print("Before Grade 1")
    elif index > 15:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def get_text():
    text = input("Text: ")
    return text


def count_letters(text):
    n = 0
    for i in range(len(text)):
        if text[i].isalpha() == True:
            n += 1
    return n


main()