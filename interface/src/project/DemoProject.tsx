import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import DemoInformation from './DemoInformation';
import SavedDataStateRestController from './SavedDataStateRestController';
import SettingsDataStateRestController from './SettingsDataStateRestController';
import PodomaticStateWebSocketController from './PodomaticStateWebSocketController';

class RestComponent extends Component {

  render() {

    return (
      <React.Fragment>
        <SavedDataStateRestController {...this.props}/>
        <SettingsDataStateRestController {...this.props}/>
        </React.Fragment>
    )
  }

}


class DemoProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {

    return (
      <MenuAppBar sectionTitle="Gestion Podomatic">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/demo/rest`} label="Podomatic" />
          <Tab value={`/${PROJECT_PATH}/demo/information`} label="Information" />
          <Tab value={`/${PROJECT_PATH}/demo/socket`} label="WebSocket Controller" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/rest`} component={RestComponent} /> 
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/information`} component={DemoInformation} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/socket`} component={PodomaticStateWebSocketController} />
          <Redirect to={`/${PROJECT_PATH}/demo/rest`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default DemoProject;
