from pathlib import Path
import argparse

try:
    from pypdf import PdfWriter, PdfReader
except ImportError:
    try:
        from PyPDF2 import PdfWriter, PdfReader
    except ImportError:
        raise SystemExit(
            "未找到 pypdf / PyPDF2。\n"
            "请先安装其一：\n"
            "  sudo apt install -y python3-pypdf2\n"
            "或在虚拟环境中：\n"
            "  pip install pypdf"
        )

TOPIC_ORDER = [
    "构造",
    "计算几何",
    "数据结构",
    "数学",
    "图论",
    "杂项",
    "字符串",
]

def natural_key(path: Path):
    return path.name

def collect_pdfs(pdf_root: Path):
    files = []
    for topic in TOPIC_ORDER:
        topic_dir = pdf_root / topic
        if not topic_dir.exists():
            continue
        topic_files = sorted(topic_dir.rglob("*.pdf"), key=natural_key)
        files.extend(topic_files)
    return files

def merge_pdfs(pdf_files, output_file: Path):
    writer = PdfWriter()
    for pdf in pdf_files:
        reader = PdfReader(str(pdf))
        for page in reader.pages:
            writer.add_page(page)
    with open(output_file, "wb") as f:
        writer.write(f)

def main():
    parser = argparse.ArgumentParser(description="合并 _pdf_export 下的多个 PDF 为一个总 PDF")
    parser.add_argument("pdf_root", help="单个 PDF 所在目录，例如 ./_pdf_export")
    parser.add_argument(
        "--output",
        default="team-wiki-merged.pdf",
        help="输出总 PDF 文件名"
    )
    args = parser.parse_args()

    pdf_root = Path(args.pdf_root).resolve()
    output_file = Path(args.output).resolve()

    if not pdf_root.exists():
        raise SystemExit(f"目录不存在：{pdf_root}")

    pdf_files = collect_pdfs(pdf_root)
    if not pdf_files:
        raise SystemExit("没有找到可合并的 PDF。")

    print("准备合并以下 PDF：")
    for pdf in pdf_files:
        print(pdf)

    merge_pdfs(pdf_files, output_file)
    print(f"\\n已生成总 PDF：{output_file}")

if __name__ == "__main__":
    main()
