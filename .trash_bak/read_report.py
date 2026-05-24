# Version: 1.1.0
from docx import Document
import sys
import os

def read_docx(file_path):
    if not os.path.exists(file_path):
        return f"File not found: {file_path}"
    doc = Document(file_path)
    full_text = []
    for para in doc.paragraphs:
        full_text.append(para.text)
    return "\n".join(full_text)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        path = sys.argv[1]
    else:
        path = r"C:\Users\Eric Wang\.gemini\Workspaces\school\one down\普物實驗\13 普朗克常數的測定\E24146107_王翊權_結報13_90分版.docx"
    
    try:
        content = read_docx(path)
        with open("C:\\Users\\Eric Wang\\report_content.txt", "w", encoding="utf-8") as f:
            f.write(content)
        print(f"Successfully read report content from {path}")
    except Exception as e:
        print(f"Error: {e}")
