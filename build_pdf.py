#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
build_pdf_print_style.py

目标：
- 扫描 team-wiki 下固定的 7 个专题目录
- .cpp 导出为“代码打印稿”风格 PDF：
  白底、等宽字体、标题、行号、自动换行、自然分页
- .md 导出为正常文档风格 PDF
- 输出到单独目录，默认 _pdf_export

依赖：
- pandoc
- pygments
- 可用浏览器（Chromium / Chrome / Edge），通过 --headless --print-to-pdf 打印
"""

from __future__ import annotations

import argparse
import html
import re
import shutil
import subprocess
import sys
from pathlib import Path
from textwrap import dedent

from pygments import highlight
from pygments.formatters import HtmlFormatter
from pygments.lexers import CppLexer


TOPIC_ORDER = [
    "构造",
    "计算几何",
    "数据结构",
    "数学",
    "图论",
    "杂项",
    "字符串",
]

INVALID_WIN_CHARS = r'<>:"/\\|?*'


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def ensure_dir(path: Path) -> None:
    path.mkdir(parents=True, exist_ok=True)


def sanitize_name(name: str) -> str:
    name = re.sub(r'[<>:"/\\|?*]', ' ', name)
    name = re.sub(r'\s+', ' ', name).strip()
    name = name.rstrip('. ')
    return name or 'untitled'


def file_uri(path: Path) -> str:
    return path.resolve().as_uri()


def find_browser(explicit: str | None = None) -> str:
    if explicit:
        if shutil.which(explicit):
            return shutil.which(explicit)  # type: ignore
        p = Path(explicit)
        if p.exists():
            return str(p.resolve())
        raise SystemExit(f"指定的浏览器不存在：{explicit}")

    candidates = [
        "msedge.exe",
        "chrome.exe",
        "chromium",
        "chromium-browser",
        "google-chrome",
        "google-chrome-stable",
        "microsoft-edge",
        "microsoft-edge-stable",
        "/mnt/c/Program Files (x86)/Microsoft/Edge/Application/msedge.exe",
        "/mnt/c/Program Files/Microsoft/Edge/Application/msedge.exe",
        "/mnt/c/Program Files/Google/Chrome/Application/chrome.exe",
    ]
    for item in candidates:
        if shutil.which(item):
            return shutil.which(item)  # type: ignore
        p = Path(item)
        if p.exists():
            return str(p.resolve())

    raise SystemExit(
        "未找到可用浏览器。请通过 --browser 指定 Chromium / Chrome / Edge 的命令或路径。"
    )


def collect_files(root: Path, suffix: str) -> list[Path]:
    files: list[Path] = []

    root_readme = root / "README.md"
    if suffix == ".md" and root_readme.exists():
        files.append(root_readme)

    for topic in TOPIC_ORDER:
        topic_dir = root / topic
        if not topic_dir.exists():
            continue
        topic_files = sorted(topic_dir.rglob(f"*{suffix}"), key=lambda p: (str(p.parent), p.name))
        files.extend(topic_files)
    return files


def write_text(path: Path, content: str) -> None:
    path.write_text(content, encoding="utf-8")


def render_pdf_from_html(browser: str, html_file: Path, pdf_file: Path) -> None:
    ensure_dir(pdf_file.parent)
    cmd = [
        browser,
        "--headless",
        "--disable-gpu",
        "--allow-file-access-from-files",
        f"--print-to-pdf={str(pdf_file.resolve())}",
        "--no-pdf-header-footer",
        file_uri(html_file),
    ]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if result.returncode != 0:
        raise RuntimeError(
            "浏览器打印 PDF 失败：\n"
            f"命令：{' '.join(cmd)}\n\n"
            f"stdout:\n{result.stdout}\n\nstderr:\n{result.stderr}"
        )
    if (not pdf_file.exists()) or pdf_file.stat().st_size == 0:
        raise RuntimeError(f"浏览器打印后未生成有效 PDF：{pdf_file}")


def build_md_css() -> str:
    return dedent("""
    @page {
      size: A4;
      margin: 16mm 14mm 18mm 14mm;
    }

    html, body {
      background: #ffffff;
      color: #222222;
      font-family: "Noto Sans CJK SC", "PingFang SC", "Microsoft YaHei", sans-serif;
      font-size: 14px;
      line-height: 1.65;
    }

    body {
      max-width: 860px;
      margin: 0 auto;
      padding: 0;
    }

    h1, h2, h3, h4 {
      color: #111111;
      line-height: 1.3;
      margin-top: 1.25em;
      margin-bottom: 0.5em;
      page-break-after: avoid;
    }

    h1 {
      font-size: 1.9em;
      border-bottom: 1px solid #e5e7eb;
      padding-bottom: 0.2em;
    }

    h2 {
      font-size: 1.45em;
      border-bottom: 1px solid #f0f0f0;
      padding-bottom: 0.15em;
    }

    h3 { font-size: 1.2em; }

    p, ul, ol, blockquote, pre, table {
      margin-top: 0.55em;
      margin-bottom: 0.55em;
    }

    code {
      font-family: "JetBrains Mono", "Consolas", "DejaVu Sans Mono", monospace;
      font-size: 0.92em;
      background: #f6f8fa;
      padding: 0.12em 0.35em;
      border-radius: 4px;
    }

    pre {
      background: #f6f8fa;
      padding: 0.9em 1em;
      border-radius: 8px;
      overflow-wrap: anywhere;
      white-space: pre-wrap;
      word-break: break-word;
      border: 1px solid #e5e7eb;
    }

    pre code {
      background: transparent;
      padding: 0;
      border-radius: 0;
      white-space: pre-wrap;
      overflow-wrap: anywhere;
      word-break: break-word;
      font-size: 0.92em;
    }

    table {
      border-collapse: collapse;
      width: 100%;
      font-size: 0.95em;
    }

    th, td {
      border: 1px solid #dcdcdc;
      padding: 0.45em 0.6em;
      vertical-align: top;
    }

    blockquote {
      color: #555555;
      border-left: 4px solid #d0d7de;
      padding-left: 1em;
      margin-left: 0;
    }

    img {
      max-width: 100%;
    }
    """)


def markdown_to_html(md_file: Path, html_file: Path, css_file: Path, root: Path) -> None:
    cmd = [
        "pandoc",
        str(md_file),
        "-f",
        "markdown",
        "-t",
        "html5",
        "-s",
        "-o",
        str(html_file),
        "--css",
        css_file.name,
        "--metadata",
        f"title={md_file.stem}",
        "--resource-path",
        str(root),
    ]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, cwd=str(html_file.parent))
    if result.returncode != 0:
        raise RuntimeError(
            "pandoc 转 HTML 失败：\n"
            f"命令：{' '.join(cmd)}\n\nstdout:\n{result.stdout}\n\nstderr:\n{result.stderr}"
        )


def build_cpp_print_html(cpp_file: Path, rel_display: str) -> str:
    formatter = HtmlFormatter(
        style="default",
        linenos="table",
        wrapcode=True,
        noclasses=False,
        cssclass="codehilite",
    )
    code_text = cpp_file.read_text(encoding="utf-8", errors="replace")
    highlighted = highlight(code_text, CppLexer(), formatter)
    pygments_css = formatter.get_style_defs(".codehilite")

    return f"""<!doctype html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<title>{html.escape(cpp_file.name)}</title>
<style>
@page {{
  size: A4;
  margin: 14mm 14mm 16mm 14mm;
}}

html, body {{
  margin: 0;
  padding: 0;
  background: #ffffff;
  color: #111111;
  font-family: "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
}}

body {{
  padding: 0;
}}

.header {{
  margin-bottom: 8px;
}}

.title {{
  font-size: 16px;
  font-weight: 700;
  color: #111111;
  margin-bottom: 2px;
}}

.subtitle {{
  font-size: 11px;
  color: #666666;
}}

.codehilite {{
  background: #ffffff;
}}

.codehilite table {{
  width: 100%;
  border-collapse: collapse;
  table-layout: fixed;
}}

.codehilite td {{
  padding: 0;
  vertical-align: top;
}}

.codehilite .linenos {{
  width: 52px;
  user-select: none;
  color: #777777;
  border-right: 1px solid #dddddd;
  padding-right: 10px;
}}

.codehilite .linenos pre {{
  text-align: right;
  margin: 0;
}}

.codehilite .code {{
  width: calc(100% - 52px);
  padding-left: 12px;
}}

.codehilite pre {{
  margin: 0;
  padding: 0;
  background: transparent !important;
  white-space: pre-wrap !important;
  overflow-wrap: anywhere !important;
  word-break: break-word !important;
}}

.codehilite code {{
  white-space: pre-wrap !important;
  overflow-wrap: anywhere !important;
  word-break: break-word !important;
}}

.codehilite .code pre {{
  font-family: "Consolas", "Cascadia Code", "JetBrains Mono", "DejaVu Sans Mono", monospace;
  font-size: 11px;
  line-height: 1.36;
  color: #111111;
}}

{pygments_css}

/* 尽量压回更接近纯打印稿的黑白风格 */
.codehilite .c, .codehilite .c1, .codehilite .cm {{ color: #666666 !important; font-style: normal !important; }}
.codehilite .k, .codehilite .kd, .codehilite .kn, .codehilite .kr {{ color: #000000 !important; font-weight: 600 !important; }}
.codehilite .nf, .codehilite .fm {{ color: #000000 !important; }}
.codehilite .s, .codehilite .s1, .codehilite .s2 {{ color: #000000 !important; }}
.codehilite .mi, .codehilite .mf, .codehilite .il {{ color: #000000 !important; }}
</style>
</head>
<body>
  <div class="header">
    <div class="title">{html.escape(rel_display)}</div>
  </div>
  {highlighted}
</body>
</html>
"""


def export_md(browser: str, md_file: Path, output_pdf: Path, root: Path, temp_dir: Path) -> None:
    css_file = temp_dir / "markdown.css"
    html_file = temp_dir / f"{sanitize_name(md_file.stem)}.html"
    write_text(css_file, build_md_css())
    markdown_to_html(md_file, html_file, css_file, root)
    render_pdf_from_html(browser, html_file, output_pdf)


def export_cpp(browser: str, cpp_file: Path, output_pdf: Path, root: Path, temp_dir: Path) -> None:
    rel = str(cpp_file.relative_to(root)).replace("\\", "/")
    html_file = temp_dir / f"{sanitize_name(cpp_file.stem)}.html"
    html_text = build_cpp_print_html(cpp_file, rel)
    write_text(html_file, html_text)
    render_pdf_from_html(browser, html_file, output_pdf)


def make_output_pdf_path(root: Path, output_dir: Path, src: Path, suffix_label: str) -> Path:
    rel = src.relative_to(root)
    out_dir = output_dir / rel.parent
    ensure_dir(out_dir)
    return out_dir / f"{sanitize_name(src.stem)} - {suffix_label}.pdf"


def main():
    parser = argparse.ArgumentParser(description="批量导出 team-wiki 下的 .md / .cpp 为 PDF")
    parser.add_argument("root", help="team-wiki 根目录")
    parser.add_argument("--output-dir", default="_pdf_export", help="输出目录，默认 _pdf_export")
    parser.add_argument("--browser", default=None, help="浏览器命令名或路径")
    parser.add_argument("--md-only", action="store_true", help="只导出 Markdown")
    parser.add_argument("--cpp-only", action="store_true", help="只导出 C++")
    parser.add_argument("--keep-html", action="store_true", help="保留中间 HTML")
    args = parser.parse_args()

    if args.md_only and args.cpp_only:
        raise SystemExit("--md-only 和 --cpp-only 不能同时使用。")

    root = Path(args.root).resolve()
    if not root.exists():
        raise SystemExit(f"根目录不存在：{root}")

    output_dir = Path(args.output_dir)
    if not output_dir.is_absolute():
        output_dir = root / output_dir
    ensure_dir(output_dir)

    browser = find_browser(args.browser)
    print(f"使用浏览器：{browser}")

    temp_base = root / "_pdf_export_html_tmp"
    if temp_base.exists():
        shutil.rmtree(temp_base)
    ensure_dir(temp_base)

    try:
        if not args.cpp_only:
            md_files = collect_files(root, ".md")
            print(f"待导出 Markdown：{len(md_files)} 个")
            for i, md_file in enumerate(md_files, 1):
                out_pdf = make_output_pdf_path(root, output_dir, md_file, "笔记")
                file_tmp_dir = temp_base / "md" / sanitize_name(md_file.stem)
                ensure_dir(file_tmp_dir)
                print(f"[MD {i}/{len(md_files)}] {md_file} -> {out_pdf}")
                export_md(browser, md_file, out_pdf, root, file_tmp_dir)

        if not args.md_only:
            cpp_files = collect_files(root, ".cpp")
            print(f"待导出 C++：{len(cpp_files)} 个")
            for i, cpp_file in enumerate(cpp_files, 1):
                out_pdf = make_output_pdf_path(root, output_dir, cpp_file, "代码")
                file_tmp_dir = temp_base / "cpp" / sanitize_name(cpp_file.stem)
                ensure_dir(file_tmp_dir)
                print(f"[CPP {i}/{len(cpp_files)}] {cpp_file} -> {out_pdf}")
                export_cpp(browser, cpp_file, out_pdf, root, file_tmp_dir)

    finally:
        if not args.keep_html and temp_base.exists():
            shutil.rmtree(temp_base)

    print(f"\n已完成，输出目录：{output_dir}")


if __name__ == "__main__":
    main()
