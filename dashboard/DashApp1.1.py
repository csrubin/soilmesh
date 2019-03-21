import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_table
#import dash_table_experiments as dt
import pandas as pd
import plotly.graph_objs as go
from plotly.graph_objs import *
from dash.dependencies import Input, Output

#external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']
app = dash.Dash(__name__)
app.css.append_css({'external_url': 'https://codepen.io/amyoshino/pen/jzXypZ.css'})


#app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

df = pd.read_csv('Book1.csv')

layout_table = dict(
    autosize=True,
    height=500,
    font=dict(color="#191A1A"),
    titlefont=dict(color="#191A1A", size='14'),
    margin=dict(
        l=35,
        r=35,
        b=35,
        t=45
    ),
    hovermode="closest",
    plot_bgcolor='#fffcfc',
    paper_bgcolor='#fffcfc',
    legend=dict(font=dict(size=10), orientation='h'),
)
layout_table['font-size'] = '18'
layout_table['margin-top'] = '120'
layout_table['margin-left'] = '60'



app.layout = html.Div([

    html.Div(
        [

        html.Div(
            [
                    html.P('Choose Time Frame:'),
                    dcc.RadioItems(
                            id = 'TimeFrame',
                            options=[
                                {'label': '1 Day', 'value': 'TD'},
                                {'label': '1 Week', 'value': 'WK'},
                                {'label': '1 Month', 'value': 'MN'},
                                {'label': '1 Year', 'value': 'YR'},
                            ],
                            value=['TD','WK','MN','YR'],
                            labelStyle={'display': 'inline-block'}
                    ),
                ],
                className='row',
                style={'margin-top': '20'}
        ),


        html.Div(
            [

                dcc.Graph(
                id='Moisture vs Time',
                figure={
                    'data': [
                        go.Scatter(
                            x=df[df['Component'] == i]['Day'],
                            y=df[df['Component'] == i]['Moisture (V/V)'],
                            mode = 'lines+markers',
                            opacity=0.7,
                            marker={
                                'size': 15,
                                'line': {'width': 0.5, 'color': 'white'}
                            },
                            name=i
                        ) for i in df.Component.unique()
                    ],
                    'layout': go.Layout(
                        xaxis={'type': 'category', 'title': 'Day'},
                        yaxis={'title': 'Moisture'},
                        margin={'l': 50, 'b': 40, 't': 50, 'r': 10},
                        legend={'x': 1, 'y': 0},
                        hovermode='closest'
                    )
                }
            )

            ],
    className='six columns'
    ),


            #html.Div(children=[
                #html.H4(children='Soil Moisture'),
                #html.Table(
                    # Header
                #    [html.Tr([html.Th(col) for col in dataframe.columns])] +

                    # Body
                #    [html.Tr([
                #        html.Td(dataframe.iloc[i][col]) for col in dataframe.columns
                #    ]) for i in range(min(len(dataframe), max_rows))]
                #)
            #])




        html.Div(
                    [
                        dash_table.DataTable(
                            id='table',
                            columns=[{"name": i, "id": i} for i in df.columns],
                            data=df.to_dict("rows"),
                            style_table={
                                'height': '200px',
                                'overflowY': 'hidden',
                                'border': 'thin lightgrey solid'
                            },
                        )
                        #html.Table(

                            # Header
                        #    [html.Tr([html.Th(col) for col in df.columns])] +

                            # Body
                        #    [html.Tr([
                        #        html.Td(df.iloc[i][col]) for col in df.columns
                        #    ]) for i in range(min(len(df), 5))]
                        #)
                    ],
                    style = layout_table,
                    className="four columns"
                ),

        html.Div([
            #html.Div(dcc.Input(id='input-box', type='Update')),
            html.Button('Update', id='button'),
            #html.Div(id='output-container-button',
            # children='Enter a value and press submit')
             ],
             className = "six columns"
             )




                ],
                className="row"
        )


])

@app.callback(
    Output('table', 'columns'))
    #Input('type', 'value'))
def update_selected_row_indices():

    columns=[{"name": i, "id": i} for i in df.columns]
    return columns


#Needs fixed
@app.callback(
    Output('Moisture vs Time', 'figure'),
    [Input('table', 'columns'),
     Input('TimeFrame', 'value')])
def update_figure(columns, value):
    dff = pd.DataFrame(columns)


    data = [
        go.Scatter(
            x=df[df['Component'] == i]['Day'],
            y=df[df['Component'] == i]['Moisture (V/V)'],
            mode = 'lines+markers',
            opacity=0.7,
            marker={
                'size': 15,
                'line': {'width': 0.5, 'color': 'white'}
            },
            name=i
        ) for i in df.Component.unique()
    ]


    return go.Figure(data=data)
#app.layout = html.Div([
#    dcc.Input(id='my-id', value='initial value', type='text'),
#    html.Div(id='my-div', children=df)
#])


#@app.callback(
#    Output(component_id='my-div', component_property='children'),
#    [Input(component_id='my-id', component_property='value')]
#)
#def update_output_div(input_value):
#    return 'You\'ve entered "{}"'.format(input_value)



if __name__ == '__main__':
    app.run_server(debug=True)
