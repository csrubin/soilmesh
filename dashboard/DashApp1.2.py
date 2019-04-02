import dash
import csv
import datetime

import dash_core_components as dcc
import dash_html_components as html
import dash_table
#import dash_table_experiments as dt
import pandas as pd
import plotly.graph_objs as go
from plotly.graph_objs import *
from dash.dependencies import Input, Output

#external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']
#app = dash.Dash(__name__)
#app.css.append_css({'external_url': 'https://codepen.io/amyoshino/pen/jzXypZ.css'})

external_stylesheets = ['https://codepen.io/amyoshino/pen/jzXypZ.css']
app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

#app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

df = pd.read_csv('WORK.csv')

#with open('WORK.csv') as f:
#    last3 = list(f)[-3:]

#print(last3)

#with open('created.csv', 'wb') as csvfile:
#    filewriter = csv.writer(csvfile, delimiter = ',')
#    filewriter.writerow(last3)



data = [] #Buffer list
with open("WORK.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reader:
        data.append(row)
        break
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 4:
            data.append(row)

with open("the_new_csv.csv", "w+") as to_file:
    writer = csv.writer(to_file, delimiter=',')
    for new_row in data:
        writer.writerow(new_row)

dfTable = pd.read_csv('the_new_csv.csv')

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
                        dcc.Interval(
                            id='interval-component',
                            interval=1*1000, # in milliseconds
                            n_intervals=0
                        ),
                        dash_table.DataTable(
                            id='table',
                            columns=[{"name": i, "id": i} for i in dfTable.columns],
                            data=dfTable.to_dict("rows"),
                            style_table={
                                #'height': '200px',
                                #'overflowY': 'hidden',
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
            #children='Enter a value and press submit')
             ],
             className = "row"
             )




                ],
                className="row"
        )


])


#@app.callback(
#    Output('table', 'data'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
#    #if n_clicks>0:
#    print(n_clicks)
#    df = pd.read_csv('WORK.csv')
#
#    data = [] #Buffer list
#    with open("WORK.csv", "rb") as input_file:
#        reader = csv.reader(input_file, delimiter=',')
#        i=0
#        for row in reader:
#            data.append(row)
#            break
#        for row in reversed(list(csv.reader(input_file))):
#            i+=1
#            if i < 5:
#                data.append(row)
#
#    with open("the_new_csv.csv", "w+") as to_file:
#        writer = csv.writer(to_file, delimiter=',')
#        for new_row in data:
#            writer.writerow(new_row)

#    dfTable = pd.read_csv('the_new_csv.csv')
#    print(dfTable)
#    rows = dfTable.to_dict("rows")
#    return rows

#@app.callback(
#    Output('table', 'columns'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
    #if n_clicks>0:
#    print(n_clicks)
#    df = pd.read_csv('WORK.csv')
#
#    data = [] #Buffer list
#    with open("WORK.csv", "rb") as input_file:
#        reader = csv.reader(input_file, delimiter=',')
#        i=0
#        for row in reader:
#            data.append(row)
#            break
#        for row in reversed(list(csv.reader(input_file))):
#            i+=1
#            if i < 5:
#                data.append(row)
#
#    with open("the_new_csv.csv", "w+") as to_file:
#        writer = csv.writer(to_file, delimiter=',')
#        for new_row in data:
#            writer.writerow(new_row)
#
#    dfTable = pd.read_csv('the_new_csv.csv')
#    print(dfTable)
#    columns=[{"name": i, "id": i} for i in dfTable.columns]
#    return columns

@app.callback(
    Output('table', 'data'),
    [Input('interval-component', 'n_intervals')])
def update_row_indices(n_intervals):
    df = pd.read_csv('WORK.csv')

    data = [] #Buffer list
    with open("WORK.csv", "rb") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reader:
            data.append(row)
            break
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 4:
                data.append(row)

    with open("the_new_csv.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        for new_row in data:
            writer.writerow(new_row)

    dfTable = pd.read_csv('the_new_csv.csv')
    print(dfTable)
    rows = dfTable.to_dict("rows")
    return rows


#    columns=[{"name": i, "id": i} for i in dfTable.columns]
#    return columns


#Needs fixed
@app.callback(
    Output('Moisture vs Time', 'figure'),
    [Input('TimeFrame', 'value'),
    Input('interval-component', 'n_intervals')])
def update_figure(value,n_intervals):
    df = pd.read_csv('WORK.csv')


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


#@app.callback(
#    Output('table', 'columns'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
#    df = pd.read_csv('WORK.csv')
#
#    data = [] #Buffer list
#    with open("WORK.csv", "rb") as input_file:
#        reader = csv.reader(input_file, delimiter=',')
#        i=0
#        for row in reader:
#            data.append(row)
#            break
#        for row in reversed(list(csv.reader(input_file))):
#            i+=1
#            if i < 5:
#                data.append(row)
#
#    with open("the_new_csv.csv", "w+") as to_file:
#        writer = csv.writer(to_file, delimiter=',')
#        for new_row in data:
#            writer.writerow(new_row)
#
#    dfTable2 = pd.read_csv('the_new_csv.csv')
#    columns=[{"name": i, "id": i} for i in dfTable2.columns]
#    return columns

#    return 'The input value was "{}" and the button has been clicked {} times'.format(
#        value,
#        n_clicks
#    )
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
    app.run_server(debug=True, port=8080)
