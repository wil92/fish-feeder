import os
import re
import minify_html

frontend_directory = 'frontend'
views_directory = 'lib/webserver/views'

base_c_header_file = r"""
#ifndef FILES_DEFININITION
#define FILES_DEFININITION

const char PAGE_CONTENT_VAR[] PROGMEM = R"PAGE(
HTML_CONTENT
)PAGE";

#endif //FILES_DEFININITION
"""

for filename in os.listdir(frontend_directory):
    if filename.endswith('.html'):
        filepath = os.path.join(frontend_directory, filename)
        with open(filepath, 'r') as file:
            content = minify_html.minify(file.read(),
                                         minify_js=True,
                                         minify_css=True,
                                         keep_comments=False,
                                         keep_ssi_comments=False,
                                         remove_processing_instructions=True)
            file_definition = '_'.join(re.split(r'[-.]', filename)).upper() + '_H'
            var_name = '_'.join(re.split(r'[-.]', filename)).lower()

            new_view_path = os.path.join(views_directory, filename.replace('.html', '.h'))
            with open(new_view_path, 'w+') as new_file:
                file_content = base_c_header_file.replace('FILES_DEFININITION', file_definition)
                file_content = file_content.replace('HTML_CONTENT', content)
                file_content = file_content.replace('PAGE_CONTENT_VAR', var_name)
                new_file.write(file_content)
                print(f'Generated {new_view_path}')
