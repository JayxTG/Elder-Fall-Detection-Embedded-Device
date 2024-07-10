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
