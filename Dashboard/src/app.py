"""
Author: Jayamadu Gammune

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

DISCLAIMER:
This code is provided for educational and informational purposes only. It is 
still under development and may contain bugs or inaccuracies. The authors are 
not responsible for any damage or loss resulting from the use of this code. Use 
at your own risk.

"""

import dash
import dash_core_components as dcc
import dash_html_components as html
from dash.dependencies import Input, Output
import dash_bootstrap_components as dbc
import threading
import time

from mqtt_client import message_queue

app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP])

app.layout = html.Div([
    dbc.Container([
        dbc.Row([
            dbc.Col(html.H1("MQTT Dashboard", className="text-center"), className="mb-5 mt-5")
        ]),
        dbc.Row([
            dbc.Col(html.Div(id="live-update-text", className="text-center"), className="mb-5 mt-5")
        ]),
        dcc.Interval(
            id='interval-component',
            interval=1*1000,  # in milliseconds
            n_intervals=0
        )
    ])
])

@app.callback(Output('live-update-text', 'children'),
              Input('interval-component', 'n_intervals'))
def update_layout(n):
    if not message_queue.empty():
        message = message_queue.get()
        return html.Div([
            html.H4(f"Latest Message: {message}")
        ])
    return html.Div([
        html.H4("Waiting for messages...")
    ])

if __name__ == '__main__':
    app.run_server(debug=True)
