from pathlib import Path
import fitz

def pdf2text(pdf_path, save_path=None):
    doc = fitz.open(pdf_path)
    text = ''
    for page in doc:
        text += page.get_text()

    if save_path:
        with open(save_path, 'w') as f:
            f.write(text)

    return text



if __name__ == '__main__':
    pdf_path = Path('datas/kimera.pdf')
    text = pdf2text(pdf_path, 'datas/kimera.txt')
    print(text)