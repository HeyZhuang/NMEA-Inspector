#!/usr/bin/env python3
"""
创建简单的SVG图标文件
运行此脚本会生成所需的图标文件
"""

import os

def create_svg_icon(filename, content, size=24):
    """创建SVG图标文件"""
    svg_content = f'''<?xml version="1.0" encoding="UTF-8"?>
<svg width="{size}" height="{size}" viewBox="0 0 {size} {size}" xmlns="http://www.w3.org/2000/svg">
{content}
</svg>'''
    
    with open(filename, 'w', encoding='utf-8') as f:
        f.write(svg_content)
    print(f"创建图标: {filename}")

def main():
    # 确保icons目录存在
    os.makedirs('icons', exist_ok=True)
    
    # 播放图标
    play_icon = '''<path d="M8 5v14l11-7z" fill="#27ae60" stroke="#1e8449" stroke-width="1"/>'''
    create_svg_icon('icons/play.png', play_icon)
    
    # 停止图标
    stop_icon = '''<rect x="6" y="6" width="12" height="12" fill="#e74c3c" stroke="#c0392b" stroke-width="1"/>'''
    create_svg_icon('icons/stop.png', stop_icon)
    
    # NMEA图标
    nmea_icon = '''<rect x="4" y="6" width="16" height="2" fill="#3498db"/>
<rect x="4" y="10" width="12" height="2" fill="#3498db"/>
<rect x="4" y="14" width="14" height="2" fill="#3498db"/>
<rect x="4" y="18" width="10" height="2" fill="#3498db"/>'''
    create_svg_icon('icons/nmea.png', nmea_icon)
    
    # 基础视图图标
    basic_icon = '''<circle cx="12" cy="12" r="8" fill="none" stroke="#3498db" stroke-width="2"/>
<path d="M12 8v8M8 12h8" stroke="#3498db" stroke-width="2"/>'''
    create_svg_icon('icons/basic.png', basic_icon)
    
    # 消息图标
    message_icon = '''<rect x="4" y="6" width="16" height="12" fill="none" stroke="#3498db" stroke-width="2"/>
<path d="M4 10l8 4 8-4" stroke="#3498db" stroke-width="2"/>'''
    create_svg_icon('icons/message.png', message_icon)
    
    # 卫星图标
    satellite_icon = '''<circle cx="12" cy="12" r="8" fill="none" stroke="#3498db" stroke-width="2"/>
<circle cx="12" cy="12" r="2" fill="#3498db"/>
<path d="M12 4l0 2M12 18l0 2M4 12l2 0M18 12l2 0" stroke="#3498db" stroke-width="1"/>'''
    create_svg_icon('icons/satellite.png', satellite_icon)
    
    # SNR图标
    snr_icon = '''<rect x="4" y="8" width="3" height="8" fill="#3498db"/>
<rect x="8" y="6" width="3" height="10" fill="#3498db"/>
<rect x="12" y="4" width="3" height="12" fill="#3498db"/>
<rect x="16" y="7" width="3" height="9" fill="#3498db"/>'''
    create_svg_icon('icons/snr.png', snr_icon)
    
    # 保存图标
    save_icon = '''<path d="M19 12v7H5v-7M17 3H7l5 5 5-5z" fill="none" stroke="#3498db" stroke-width="2"/>'''
    create_svg_icon('icons/save.png', save_icon)
    
    # 清空图标
    clear_icon = '''<circle cx="12" cy="12" r="8" fill="none" stroke="#e74c3c" stroke-width="2"/>
<path d="M8 8l8 8M16 8l-8 8" stroke="#e74c3c" stroke-width="2"/>'''
    create_svg_icon('icons/clear.png', clear_icon)
    
    # 刷新图标
    refresh_icon = '''<path d="M3 12a9 9 0 0 1 9-9 9.75 9.75 0 0 1 6.74 2.74L21 8M21 12a9 9 0 0 1-9 9 9.75 9.75 0 0 1-6.74-2.74L3 16" fill="none" stroke="#3498db" stroke-width="2"/>
<path d="M21 8v4h-4M3 16v-4h4" stroke="#3498db" stroke-width="2"/>'''
    create_svg_icon('icons/refresh.png', refresh_icon)
    
    print("\n所有图标已创建完成！")
    print("注意：这些是SVG文件，如果需要PNG格式，请使用图像转换工具。")

if __name__ == "__main__":
    main()
