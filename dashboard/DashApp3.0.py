import dash
import csv

import dash_core_components as dcc
import dash_html_components as html
import dash_table
#import dash_table_experiments as dt
import numpy as np
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

#df = pd.read_csv('WORK2.csv')

df1 = pd.read_csv('NODE1.csv')
df2 = pd.read_csv('NODE2.csv')
df3 = pd.read_csv('NODE3.csv')
#with open('WORK2.csv') as f:
#    last3 = list(f)[-3:]

#print(last3)

#with open('created.csv', 'wb') as csvfile:
#    filewriter = csv.writer(csvfile, delimiter = ',')
#    filewriter.writerow(last3)



data = [] #Buffer list
data.append(['nodeID','year','month','day','hour','min','sec','moisture','latitude','longitude','altitude','satellites'])
with open("NODE1.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 2:
            data.append(row)

with open("NODE2.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 2:
            data.append(row)

with open("NODE3.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 2:
            data.append(row)

with open("the_new_csv.csv", "w+") as to_file:
    writer = csv.writer(to_file, delimiter=',')
    row = data[0]
    #print(row)
    writer.writerow(row)

    for new_row in data:
        if new_row[0] == '1':
           writer.writerow(new_row)


    for new_row in data:
        if new_row[0] == '2':
            writer.writerow(new_row)

    for new_row in data:
        if new_row[0] == '3':
            writer.writerow(new_row)

data2 = [] #Buffer list
data2.append(['nodeID','year','month','day','hour','min','sec','moisture','latitude','longitude','altitude','satellites'])
with open("NODE1.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    for row in list(csv.reader(input_file)):
        data2.append(row)

with open("NODE2.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    for row in list(csv.reader(input_file)):
        data2.append(row)


with open("NODE3.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    for row in list(csv.reader(input_file)):
        data2.append(row)


with open("the_new_csv2.csv", "w+") as to_file:
    writer = csv.writer(to_file, delimiter=',')
    for new_row in data2:
        if new_row[7] != str(0):
           writer.writerow(new_row)



shaz13_custom_style = "mapbox://styles/shaz13/cjiog1iqa1vkd2soeu5eocy4i"
dfTable = pd.read_csv('the_new_csv.csv')
df = pd.read_csv('the_new_csv2.csv')

#dfSecond

#dfMinute = df
#for i in range (2,len(df['min'])):
#    dfMinute.drop(dfMinute.index[[i]])
#    if dfMinute['min'][i] == dfMinute['min'][i-1]:
#        dfMinute.drop([i])
#print(dfMinute)
#dfHour
#dfDay
#dfWeek

#dfMap = pd.read_csv('https://raw.githubusercontent.com/plotly/datasets/master/2011_february_us_airport_traffic.csv')
#dfMap['text'] = dfMap['airport'] + '' + dfMap['city'] + ', ' + dfMap['state'] + '' + 'Arrivals: ' + dfMap['cnt'].astype(str)

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
                                {'label': '10 Seconds', 'value': 'SE'},
                                {'label': '1 Minute', 'value': 'MN'},
                                {'label': '1 Hour', 'value': 'HR'},
                                {'label': '1 Day', 'value': 'DY'},
                                {'label': '1 Week', 'value': 'WK'},
                            ],
                            value=['SE','MN','HR','DY','WK'],
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
                            x= tuple(range(0,9)),  #df[df['nodeID'] == i]['sec'][-10:],
                            y=df[df['nodeID'] == i]['moisture'][-10:],
                            mode = 'lines+markers',
                            opacity=0.7,
                            marker={
                                'size': 15,
                                'line': {'width': 0.5, 'color': 'white'}
                            },
                            name=str(i)
                        ) for i in df.nodeID.unique()
                    ],
                    'layout': go.Layout(
                        xaxis={'title': 'Sec'},
                        yaxis={'title': 'Moisture'},
                        margin={'l': 50, 'b': 40, 't': 50, 'r': 10},
                        legend={'x': 1, 'y': 0},
                        hovermode='closest'
                    )
                }
            ),
                dcc.Graph(
                id='Location',
                figure={

                    #set the geo=spatial data
                    'data': [
                        go.Scattermapbox(
                            lon = dfTable[dfTable['nodeID'] == i]['longitude'],
                            lat = dfTable[dfTable['nodeID'] == i]['latitude'],
                            #lat=['45.5017'],
                            #lon=['-73.5673'],
                            mode='markers',
                            marker=go.scattermapbox.Marker(
                                size=14
                            ),
                            text=['Columbus'],
                        ) for i in df.nodeID.unique()
                    ],

                    'layout': go.Layout(
                        autosize=True,
                        hovermode='closest',
                        mapbox=go.layout.Mapbox(
                            accesstoken='pk.eyJ1IjoibWFweWF6OTciLCJhIjoiY2p1Y2Mxdm0wMGo1NjRlcXJkYWtxcGw1MyJ9.MRkvu2qIMLsNgJHgUnOk1A',
                            bearing=0,
                            center=go.layout.mapbox.Center(
                                lat=40.002170,
                                lon=-83.016694
                            ),
                            pitch=0,
                            zoom=10
                        ),
                    )#,
                    #'layout': go.Layout(
                        #xaxis={'title': 'Lattitude'}, #'type': 'category',
                        #yaxis={'title': 'Longitude'},
                        #margin={'l': 50, 'b': 40, 't': 50, 'r': 10},
                        #legend={'x': 1, 'y': 0},
                        #hovermode='closest'
                    #)
                }
            )#,

            #dcc.Slider(
            #    id='Zoomer',
            #    min=8,
            #    max=18,
            #    step=0.1,
            #    value=15,
            #)


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
                            columns=[{"name": 'nodeID', "id": 'nodeID'}, {"name": 'moisture', "id": 'moisture'}, {"name": 'latitude', "id": 'latitude'}, {"name": 'longitude', "id": 'longitude'}, {"name": 'altitude', "id": 'altitude'} ],#for i in dfTable.columns],
                            data=dfTable.to_dict("rows"),
                            style_header={
                                'backgroundColor': 'Yellow',
                                },
                            #deletable=True,
                            style_cell={
                                #'backgroundColor': 'green',
                                #'height': '200px',
                                #'overflowY': 'hidden',
                                #'border': 'thin lightgrey solid'
                            },
                            style_data_conditional = [
                            {
                                'if': {
                                    'column_id': 'moisture',
                                    'filter': 'moisture < num(0.1)'
                                },
                                    'backgroundColor': 'red',
                                    'color': 'white',
                            }
                            ]
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
#    df = pd.read_csv('WORK2.csv')
#
#    data = [] #Buffer list
#    with open("WORK2.csv", "rb") as input_file:
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
#    df = pd.read_csv('WORK2.csv')
#
#    data = [] #Buffer list
#    with open("WORK2.csv", "rb") as input_file:
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
    df1 = pd.read_csv('NODE1.csv')
    df2 = pd.read_csv('NODE2.csv')
    df3 = pd.read_csv('NODE3.csv')
    #with open('WORK2.csv') as f:
    #    last3 = list(f)[-3:]

    #print(last3)

    #with open('created.csv', 'wb') as csvfile:
    #    filewriter = csv.writer(csvfile, delimiter = ',')
    #    filewriter.writerow(last3)



    data = [] #Buffer list
    data.append(['nodeID','year','month','day','hour','min','sec','moisture','latitude','longitude','altitude','satellites'])
    with open("NODE1.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 2:
                data.append(row)

    with open("NODE2.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 2:
                data.append(row)

    with open("NODE3.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 2:
                data.append(row)

    with open("the_new_csv.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        row = data[0]
        #print(row)
        writer.writerow(row)

        for new_row in data:
            if new_row[0] == '1':
               writer.writerow(new_row)


        for new_row in data:
            if new_row[0] == '2':
                writer.writerow(new_row)

        for new_row in data:
            if new_row[0] == '3':
                writer.writerow(new_row)

    data2 = [] #Buffer list
    data2.append(['nodeID','year','month','day','hour','min','sec','moisture','latitude','longitude','altitude','satellites'])
    with open("NODE1.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)

    with open("NODE2.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)


    with open("NODE3.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)


    with open("the_new_csv2.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        for new_row in data2:
            if new_row[7] != str(0):
               writer.writerow(new_row)


    dfTable = pd.read_csv('the_new_csv.csv')
    df = pd.read_csv('the_new_csv2.csv')
    #print(dfTable)
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
    data2 = [] #Buffer list
    data2.append(['nodeID','year','month','day','hour','min','sec','moisture','latitude','longitude','altitude','satellites'])
    with open("NODE1.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)

    with open("NODE2.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)


    with open("NODE3.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)


    with open("the_new_csv2.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        for new_row in data2:
            if new_row[7] != str(0):
               writer.writerow(new_row)

    df = pd.read_csv('the_new_csv2.csv')


    if value == 'DY':
        data = [
            go.Scatter(

                x=tuple(np.linspace(0,24,60)),#df[df['nodeID'] == i]['hour'][-24:],
                y=df[df['nodeID'] == i]['moisture'][-86400::1440],
                mode = 'lines',
                opacity=0.7,
                marker={
                    'size': 15,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]
        layout = go.Layout(
                xaxis={ 'title': 'Hour'},
                yaxis={'title': 'Moisture'}
                )

    elif value == 'WK':
        data = [
            go.Scatter(
                x=np.linspace(0,7,60),#df[df['nodeID'] == i]['sec'][-60:],
                y=df[df['nodeID'] == i]['moisture'][-604800::10080],#[0::1000],
                mode = 'lines',
                opacity=0.7,
                marker={
                    'size': 5,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]
        layout = go.Layout(
                xaxis={'title': 'Day'},
                yaxis={'title': 'Moisture'}
                )
    elif value == 'MN':
        data = [
            go.Scatter(
                x=tuple(range(0,59)),#df[df['nodeID'] == i]['sec'][-60:],
                y=df[df['nodeID'] == i]['moisture'][-60:],
                mode = 'lines+markers',
                opacity=0.7,
                marker={
                    'size': 5,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]
        layout = go.Layout(
                xaxis={'title': 'Second'},
                yaxis={'title': 'Moisture'}
                )
    elif value == 'HR':
        data = [
            go.Scatter(
                x=tuple(np.linspace(0,60,60)),#df[df['nodeID'] == i]['min'][-60:],
                y=df[df['nodeID'] == i]['moisture'][-3600::60],
                mode = 'lines+markers',
                opacity=0.7,
                marker={
                    'size': 1,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]
        layout = go.Layout(
                xaxis={'title': 'Minute'},
                yaxis={'title': 'Moisture'}
                )
    else:
        data = [
            go.Scatter(
                x= tuple(range(0,9)),  #df[df['nodeID'] == i]['sec'][-10:],
                y=df[df['nodeID'] == i]['moisture'][-10:],
                mode = 'lines+markers',
                opacity=0.7,
                marker={
                    'size': 15,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]
        layout = go.Layout(
                xaxis={'title': 'Second'},
                yaxis={'title': 'Moisture'}
                )

    return go.Figure(data=data,layout=layout)

@app.callback(
    Output('Location', 'figure'),
    [Input('interval-component', 'n_intervals')]) #Input('Zoomer', 'value')
def update_figure(n_intervals):
    #df = pd.read_csv('WORK2.csv')

    data = [
        go.Scattermapbox(
            lon = dfTable[dfTable['nodeID'] == i]['longitude'],
            lat = dfTable[dfTable['nodeID'] == i]['latitude']
        )
    ]
    #layout = go.Layout(
    #    autosize=True,
    #    hovermode='closest',
    #    mapbox=go.layout.Mapbox(
    #        zoom=value
    #    ),
    #)


    return go.Figure(data=data)#,layout = layout


#@app.callback(
#    Output('table', 'columns'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
#    df = pd.read_csv('WORK2.csv')
#
#    data = [] #Buffer list
#    with open("WORK2.csv", "rb") as input_file:
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
    app.run_server(debug=True, port=8050)
