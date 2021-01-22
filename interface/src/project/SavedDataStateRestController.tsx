import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box,TextField, FormControlLabel, Switch, Paper} from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import RefreshIcon from '@material-ui/icons/Refresh';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent} from '../components';

import { SavedDataState } from './types';
import SettingsDataStateRestControllerForm from'./SettingsDataStateRestController' ;
import { render } from 'react-dom';
//import { TextFieldsOutlined } from '@material-ui/icons';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "SavedDataState";

type SavedDataStateRestControllerProps = RestControllerProps<SavedDataState>;

class SavedDataStateRestController extends Component<SavedDataStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='Historique podomatic' titleGutter >
        <RestFormLoader
          {...this.props}
          render={props => (
            <SavedDataStateRestControllerForm {...props} />
          )}
        />
        <FormButton startIcon={<RefreshIcon />} variant="contained" color="primary" onClick={this.props.loadData}>
          Rafraichir
        </FormButton>
      </SectionContent>
    )
  }

}

export default restController(LIGHT_SETTINGS_ENDPOINT, SavedDataStateRestController);

type SavedDataStateRestControllerFormProps = RestFormProps<SavedDataState>;

function refreshPage() {

}

function SavedDataStateRestControllerForm(props: SavedDataStateRestControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    
      <><Typography variant="subtitle2" gutterBottom>
      Temps total de spray : {data.temps_total_spray} heures ou {data.temps_total_spray*60} minutes
    </Typography>
      <Typography variant="subtitle2" gutterBottom>
        Nombre total de passages: {data.nb_total_passage}
      </Typography></>

  );
}
