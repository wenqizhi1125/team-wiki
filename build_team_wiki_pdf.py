#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import sys
import tempfile
from collections import defaultdict
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Tuple

TOPIC_ORDER = [
    "构造",
    "计算几何",
    "数据结构",
    "数学",
    "图论",
    "杂项",
    "字符串",
]

FONT_CANDIDATES = [
    "Noto Sans CJK SC",
    "Source Han Sans SC",
    "WenQuanYi Zen Hei",
    "AR PL UMing CN",
    "Microsoft YaHei",
    "SimSun",
]

CODE_EXTENSIONS = {".cpp", ".cc", ".cxx", ".hpp", ".h", ".hh", ".hxx"}
MARKDOWN_EXTENSIONS = {".md", ".markdown"}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="将 team-wiki 按专题整合为一个总 Markdown，并可进一步导出为 PDF。"
    )
    parser.add_argument(
        "root",
        nargs="?",
        default=".",
        help="team-wiki 根目录，默认当前目录",
    )
    parser.add_argument(
        "-o",
        "--output",
        default="team-wiki-final.pdf",
        help="输出 PDF 文件名，默认 team-wiki-final.pdf",
    )
    parser.add_argument(
        "--output-md",
        default="team-wiki-final.md",
        help="输出合并后的 Markdown 文件名，默认 team-wiki-final.md",
    )
    parser.add_argument(
        "--font",
        default="",
        help="手动指定中文字体名，例如 'Noto Sans CJK SC'",
    )
    parser.add_argument(
        "--no-toc",
        action="store_true",
        help="不生成目录",
    )
    parser.add_argument(
        "--md-only",
        action="store_true",
        help="只生成总 Markdown，不调用 pandoc 导出 PDF",
    )
    return parser.parse_args()


def check_root(root: Path) -> None:
    if not root.exists() or not root.is_dir():
        raise SystemExit(f"错误：目录不存在 - {root}")


FENCE_RE = re.compile(r"^```")
HEADING_RE = re.compile(r"^(#{1,6})(\s+.*)$")


def demote_markdown_headings(text: str, by: int = 1) -> str:
    """将 Markdown 标题整体下沉，避免和总文档标题层级冲突。"""
    lines = text.splitlines()
    out: List[str] = []
    in_fence = False
    for line in lines:
        if FENCE_RE.match(line.strip()):
            in_fence = not in_fence
            out.append(line)
            continue
        if not in_fence:
            m = HEADING_RE.match(line)
            if m:
                level = min(6, len(m.group(1)) + by)
                line = "#" * level + m.group(2)
        out.append(line)
    return "\n".join(out).strip()



def slug_display_name(rel_no_suffix: Path) -> str:
    """把相对路径变成更适合展示的标题。"""
    parts = list(rel_no_suffix.parts)
    return " / ".join(parts)



def collect_entries(topic_dir: Path) -> Dict[Path, Dict[str, Path]]:
    groups: Dict[Path, Dict[str, Path]] = defaultdict(dict)
    if not topic_dir.exists() or not topic_dir.is_dir():
        return groups

    for path in topic_dir.rglob("*"):
        if not path.is_file():
            continue
        if path.name.startswith("."):
            continue
        suffix = path.suffix.lower()
        if suffix not in CODE_EXTENSIONS and suffix not in MARKDOWN_EXTENSIONS:
            continue
        rel = path.relative_to(topic_dir)
        rel_no_suffix = rel.with_suffix("")
        if suffix in MARKDOWN_EXTENSIONS:
            groups[rel_no_suffix]["md"] = path
        elif suffix in CODE_EXTENSIONS:
            groups[rel_no_suffix]["cpp"] = path
    return groups



def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")



def build_markdown(root: Path) -> str:
    lines: List[str] = []
    title = root.name or "team-wiki"

    lines.append(f"# {title} 最终版手册")
    lines.append("")
    lines.append(f"> 生成时间：{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    lines.append("")

    root_readme = root / "README.md"
    if root_readme.exists():
        lines.append("## 仓库说明")
        lines.append("")
        content = read_text(root_readme).strip()
        if content:
            lines.append(demote_markdown_headings(content, by=2))
            lines.append("")

    for topic in TOPIC_ORDER:
        topic_dir = root / topic
        if not topic_dir.exists():
            continue

        groups = collect_entries(topic_dir)
        if not groups:
            continue

        lines.append(f"# {topic}")
        lines.append("")

        topic_readme = topic_dir / "README.md"
        if topic_readme.exists():
            topic_intro = read_text(topic_readme).strip()
            if topic_intro:
                lines.append(demote_markdown_headings(topic_intro, by=1))
                lines.append("")

        for rel_no_suffix in sorted(groups.keys(), key=lambda p: str(p)):
            entry = groups[rel_no_suffix]
            display = slug_display_name(rel_no_suffix)
            lines.append(f"## {display}")
            lines.append("")

            md_path = entry.get("md")
            cpp_path = entry.get("cpp")

            if md_path:
                md_text = read_text(md_path).strip()
                if md_text:
                    lines.append(demote_markdown_headings(md_text, by=2))
                    lines.append("")

            if cpp_path:
                lines.append("### 参考代码")
                lines.append("")
                lines.append("```cpp")
                lines.append(read_text(cpp_path).rstrip())
                lines.append("```")
                lines.append("")

    return "\n".join(lines).rstrip() + "\n"



def detect_font(user_font: str) -> str:
    if user_font:
        return user_font
    if shutil.which("fc-match") is None:
        return ""
    for font in FONT_CANDIDATES:
        try:
            result = subprocess.run(
                ["fc-match", "-f", "%{family}\n", font],
                capture_output=True,
                text=True,
                check=False,
            )
        except Exception:
            continue
        family = result.stdout.strip()
        if family and family.lower() != "":
            return font
    return ""



def ensure_tool(name: str, install_hint: str) -> None:
    if shutil.which(name) is None:
        raise SystemExit(
            f"错误：未找到 {name}\n"
            f"建议先安装：{install_hint}"
        )



def render_pdf(markdown_path: Path, pdf_path: Path, font_name: str, toc: bool) -> None:
    ensure_tool("pandoc", "sudo apt install pandoc")
    ensure_tool("xelatex", "sudo apt install texlive-xetex texlive-latex-recommended texlive-latex-extra")

    cmd = [
        "pandoc",
        str(markdown_path),
        "-o",
        str(pdf_path),
        "--pdf-engine=xelatex",
        "-V",
        "geometry:margin=1.8cm",
        "-V",
        "colorlinks=true",
        "--highlight-style=tango",
    ]

    if toc:
        cmd.append("--toc")
        cmd.extend(["--toc-depth=2"])

    if font_name:
        cmd.extend(["-V", f"mainfont={font_name}"])
        cmd.extend(["-V", f"CJKmainfont={font_name}"])
    cmd.extend(["-V", "monofont=DejaVu Sans Mono"])

    proc = subprocess.run(cmd, capture_output=True, text=True, check=False)
    if proc.returncode != 0:
        raise SystemExit(
            "导出 PDF 失败。\n"
            f"命令：{' '.join(cmd)}\n\n"
            f"stdout:\n{proc.stdout}\n\n"
            f"stderr:\n{proc.stderr}"
        )



def main() -> None:
    args = parse_args()
    root = Path(args.root).resolve()
    check_root(root)

    output_pdf = (root / args.output).resolve() if not Path(args.output).is_absolute() else Path(args.output)
    output_md = (root / args.output_md).resolve() if not Path(args.output_md).is_absolute() else Path(args.output_md)

    merged_md = build_markdown(root)
    output_md.write_text(merged_md, encoding="utf-8")
    print(f"已生成总 Markdown：{output_md}")

    if args.md_only:
        return

    font_name = detect_font(args.font)
    if not font_name:
        print("警告：未自动检测到常见中文字体。若导出 PDF 出现中文乱码，请手动加 --font 指定字体。")

    render_pdf(output_md, output_pdf, font_name, toc=not args.no_toc)
    print(f"已生成 PDF：{output_pdf}")
    if font_name:
        print(f"使用字体：{font_name}")


if __name__ == "__main__":
    main()
